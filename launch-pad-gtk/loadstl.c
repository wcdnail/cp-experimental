#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include "loadstl.h"
#include "logbox.h"
#include <errno.h>

//---------------------------------------------------------------------------------------------------------------------

static PGMesh stlLoadAsc(const gchar *pathname, GInputStream *istm);
static PGMesh stlLoadBin(const gchar *pathname, GInputStream *istm);

//---------------------------------------------------------------------------------------------------------------------

PGMesh stlLoadFile(const gchar *pathname) 
{
    return (NULL);
}

PGMesh stlLoadResource(const gchar *pathname)
{
    const gchar *errorTitle = "unknown";
    GError           *error = NULL;
    PGMesh           result = NULL;
    GInputStream      *istm = g_resources_open_stream(pathname, 0, &error);
    if (!istm) {
        errorTitle = "resource open";
        goto onError;
    }
    result = stlLoadAsc(pathname, istm);
    goto noError;
onError:
    lgTrace(LG_ERROR, "LOADSTLRES [%s] %s ERROR: [%d] %s\n", 
        pathname, 
        errorTitle,
        error ? error->code : errno, 
        error ? error->message : strerror(errno));
noError:
    if (istm) {
        g_object_unref(istm);
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------

enum _ASTLParserState
{
    _ASTL_INVALID = -5,
    _ASTL_SKIP_LINE = 0,
    _ASTL_BEG_FILE,
    _ASTL_BEG_FACET,
    _ASTL_BEG_OUTER_LOOP,
    _ASTL_VERTEX,
    _ASTL_END_OUTER_LOOP,
    _ASTL_END_FACET,
    _ASTL_END_FILE,
};

static const gchar* astl_parser_state_str(gint state) 
{
    switch (state) {
    case _ASTL_INVALID:         return ("UNKNW");
    case _ASTL_SKIP_LINE:       return ("SKIPL");
    case _ASTL_BEG_FILE:        return ("FILEB");
    case _ASTL_BEG_FACET:       return ("FACEB");
    case _ASTL_BEG_OUTER_LOOP:  return ("LOOPB");
    case _ASTL_VERTEX:          return ("VERTX");
    case _ASTL_END_OUTER_LOOP:  return ("LOOPE");
    case _ASTL_END_FACET:       return ("FACEE");
    case _ASTL_END_FILE:        return ("FILEE");
    }
    lgTrace(LG_ERROR, "INVALID ASTL parser state: %d\n", state);
    g_assert_not_reached();
    return (NULL);
}

static gsize skipTailSpecials(gchar *arg, gsize len)
{
    gchar *tail = arg + len;
    while ((tail > arg) && (*tail < ' ')) {
        --tail;
    }
    *(++tail) = 0;
    return ((gsize)tail - (gsize)arg);
}

static gboolean astlCheckToken(gchar *token, gsize len, const gchar *lexeme, gsize lexsize, gint resState, gint *state, gchar **arg, gsize *arglen)
{
    if (0 != g_ascii_strncasecmp(token, lexeme, lexsize)) {
        return (FALSE);
    }
    if (arg) {
        gsize skipsize = lexsize + 1;
        gsize     argl = 0;
        *arg = token + skipsize;
        argl = skipTailSpecials(*arg, len - skipsize);
        if (arglen) {
            (*arglen) = argl;
        }
    }
    (*state) = resState;
    return (TRUE);
}

static gint astlGetParserState(gchar *line, gsize len, gchar **arg, gsize *arglen)
{
    gint  state = _ASTL_INVALID;
    char *token = NULL;
    if (len < 3) {
        return _ASTL_SKIP_LINE;
    }
    token = line;
    while (*token && *token == ' ') {
        ++token;
    }
    switch(g_ascii_tolower(*token)) {
    case 's':
        astlCheckToken(token, len, "solid", 5, _ASTL_BEG_FILE, &state, arg, arglen);
        break;
    case 'e':
        if (astlCheckToken(token, len, "end", 3, _ASTL_INVALID, &state, NULL, NULL)) {
            if (astlCheckToken(token + 3, len - 3, "facet", 5, _ASTL_END_FACET, &state, NULL, NULL)) {
                return state;
            }
            else if (astlCheckToken(token + 3, len - 3, "loop", 4, _ASTL_END_OUTER_LOOP, &state, NULL, NULL)) {
                return state;
            }
            else if (astlCheckToken(token + 3, len - 3, "solid", 5, _ASTL_END_FILE, &state, NULL, NULL)) {
                return state;
            }
        }
        break;
    case 'f':
        if (astlCheckToken(token, len, "facet", 5, _ASTL_BEG_FACET, &state, NULL, NULL)) {
            astlCheckToken(token + 6, len - 6, "normal", 6, _ASTL_BEG_FACET, &state, arg, arglen);
            return state;
        }
        break;
    case 'o':
        if (astlCheckToken(token, len, "outer loop", 10, _ASTL_BEG_OUTER_LOOP, &state, NULL, NULL)) {
            return state;
        }
        break;
    case 'v':
        if (astlCheckToken(token, len, "vertex", 6, _ASTL_VERTEX, &state, arg, arglen)) {
            return state;
        }
        break;
    }
    return state;
}

static PGMesh stlLoadAsc(const gchar *pathname, GInputStream *istm) 
{
    const gchar    *errorTitle = "unknown";
    const gchar *parseErrorMsg = "invalid parser state";
    GError              *error = NULL;
    PGMesh              result = NULL;
    gsize               readed = 0;
    GDataInputStream    *idata = NULL;
    gchar                *line = NULL;
    guint              lineNum = 1;
    gint                 state = _ASTL_INVALID;
    gint             prevState = _ASTL_BEG_FILE;
    guint               vindex = 0;
    GArray          *triangles = NULL;
    PGTriangle        triangle = NULL;
    GTriangle      triangleBuf = {0};
    if (!istm) {
        errorTitle = "input stream validation";
        goto onError;
    }
    idata = g_data_input_stream_new(istm);
    if (!idata) {
        errorTitle = "data input stream creation";
        goto onError;
    }
    line = g_data_input_stream_read_line(idata, &readed, NULL, &error);
    if (!line) {
        errorTitle = "input stream read header";
        goto onError;
    }
    if (0 != g_ascii_strncasecmp(line, "solid", 5)) {
        g_object_unref(idata);
        return stlLoadBin(pathname, istm);
    }
    triangles = g_array_new(FALSE, TRUE, sizeof(GTriangle));
    if (!triangles) {
        errorTitle = "temp triangles array allocating";
        goto onError;
    }
    result = meshNew(pathname);
    if (!result) {
        errorTitle = "mesh allocating";
        goto onError;
    }
    do {
        gchar   *arg = NULL;
        gsize arglen = 0;
        state = astlGetParserState(line, readed, &arg, &arglen);
        switch (state) {
        case _ASTL_BEG_FILE: {
            result->description = g_string_new(arg);
            break;
        }
        case _ASTL_BEG_FACET: {
            if (triangle) {
                parseErrorMsg = "current triangle not NULL";
                goto parserError;
            }
            triangle = &triangleBuf;
            if (arg && !vertexFromString(&triangle->normal, arg, arglen)) {
                parseErrorMsg = "parsing normal error";
                goto parserError;
            }
            break;
        }
        case _ASTL_BEG_OUTER_LOOP:   
            if (prevState != _ASTL_BEG_FACET) {
                parseErrorMsg = "outer loop is outside facet";
                goto parserError;
            }
            vindex = 0;
            break;
        case _ASTL_VERTEX: {
            if (!triangle) {
                parseErrorMsg = "current triangle is NULL";
                goto parserError;
            }
            if (vindex > 2) {
                parseErrorMsg = "vertex count more than 3";
                goto parserError;
            }
            if (!vertexFromString(&triangle->vertex[vindex], arg, arglen)) {
                parseErrorMsg = "parsing vertex error";
                goto parserError;
            }
            ++vindex;
            break;
        }
        case _ASTL_END_OUTER_LOOP:
            if (prevState != _ASTL_VERTEX) {
                parseErrorMsg = "outer loop ends unexpectedly";
                goto parserError;
            }
            if (!triangle) {
                parseErrorMsg = "current triangle is NULL";
                goto parserError;
            }
            g_array_append_vals(triangles, triangle, 1);
            triangle = NULL;
            break;
        case _ASTL_END_FACET: break;
        case _ASTL_END_FILE:  break;
        case _ASTL_SKIP_LINE: break;
        default:
parserError:        
            lgTrace(LG_ERROR, "LOADSTLA %s(%d): [%s] %s\n", pathname, lineNum, astl_parser_state_str(state), parseErrorMsg);
            errorTitle = NULL;
            goto onError;
        }
        if (state > _ASTL_SKIP_LINE) {
            prevState = state;
        }
        ++lineNum;
        line = g_data_input_stream_read_line(idata, &readed, NULL, &error);
    }
    while (readed > 0);
    result->triangles = triangles;
    triangles = NULL;
    lgTrace(LG_ASSERT, "LOADSTLA '%s' %s [T:%d]\n", result->name->str, result->description->str, result->triangles->len);
    goto noError;
onError:
    if (errorTitle) {
        lgTrace(LG_ERROR, "LOADSTLA [%s] %s ERROR: [%d] %s\n", 
            pathname,
            errorTitle,
            error ? error->code : errno, 
            error ? error->message : strerror(errno));
    }
    meshFree(result);
    result = NULL;
noError:
    if (triangles) {
        g_array_free(triangles, TRUE);
    }
    if (idata) {
        g_object_unref(idata);
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------

static PGMesh stlLoadBin(const gchar *pathname, GInputStream *istm)
{
    return NULL;
}
