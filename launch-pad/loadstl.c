#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include "loadstl.h"
#include "filemanip.h"
#include "logbox.h"

PGMesh stlLoadFile(const gchar *pathname) 
{
    return NULL;
}

PGMesh stlLoadResource(const gchar *pathname)
{
    const gchar *errorTitle = "unknown";
    GError           *error = NULL;
    GInputStream      *istm = NULL;
    PGMesh           result = NULL;
    istm = g_resources_open_stream(pathname, 0, &error);
    if (!istm) {
        errorTitle = "resource open";
        goto onError;
    }
    result = stlLoadAsc(pathname, istm);
    goto noError;
onError:
    logBoxTrace(LOGBOX_ERROR, "LOADSTLRES [%s] %s ERROR: [%d] %s\n", 
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

static gint astl_get_parser_state(const gchar *line, gssize len, const gchar **arg)
{
    const char *token;
    if (len < 3) {
        return _ASTL_SKIP_LINE;
    }
    token = line;
    while (*token && *token == ' ') {
        ++token;
    }
    switch(g_ascii_tolower(*token)) {
    case 'e':
        if (0 == g_ascii_strncasecmp(token, "end", 3)) {
            if (0 == g_ascii_strncasecmp(token + 3, "facet", 5)) {
                return _ASTL_END_FACET;
            }
            else if (0 == g_ascii_strncasecmp(token + 3, "loop", 4)) {
                return _ASTL_END_OUTER_LOOP;
            }
            else if (0 == g_ascii_strncasecmp(token + 3, "solid", 5)) {
                return _ASTL_END_FILE;
            }
        }
        break;
    case 'f':
        if (0 == g_ascii_strncasecmp(token, "facet", 5)) {
            return _ASTL_BEG_FACET;
        }
    case 'o':
        if (0 == g_ascii_strncasecmp(token, "outer loop", 10)) {
            return _ASTL_BEG_OUTER_LOOP;
        }
        break;
    case 'v':
        if (0 == g_ascii_strncasecmp(token, "vertex", 6)) {
            (*arg) = token + 7;
            return _ASTL_VERTEX;
        }
        break;
    }
    return _ASTL_INVALID;
}

static gboolean astl_get_vertex(GLfloat *vdata, const gchar *arg)
{
    gchar **varg = g_strsplit(arg, " ", 3);
    if (!varg) {
        return FALSE;
    }
    vdata[0] = (GLfloat)g_ascii_strtod(varg[0], NULL);
    vdata[1] = (GLfloat)g_ascii_strtod(varg[1], NULL);
    vdata[2] = (GLfloat)g_ascii_strtod(varg[2], NULL);
    g_strfreev(varg);
    return TRUE;
}

PGMesh stlLoadAsc(const gchar *pathname, GInputStream *istm) 
{
    const gchar  *errorTitle = "unknown";
    const gchar *parserTitle = "invalid parser state";
    GError            *error = NULL;
    PGMesh            result = NULL;
    gssize            readed = 0;
    GDataInputStream  *idata = NULL;
    gchar              *line = NULL;
    guint            lineNum = 2; // skip header line
    gint           prevState = _ASTL_BEG_FILE;
    guint             vindex = 0;
    guint             iindex = 0;
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
    result = meshNew(pathname);
    if (!result) {
        errorTitle = "mesh allocating";
        goto onError;
    }
    do {
        gint       state = 0;
        const gchar *arg = NULL;
        readed = 0;
        line = g_data_input_stream_read_line(idata, &readed, NULL, &error);
        if (readed < 0) {
            errorTitle = "data input stream read";
            goto onError;
        }
        state = astl_get_parser_state(line, readed, &arg);
        switch (state) {
        case _ASTL_BEG_FACET: break; // TODO: parse normal & check prevState
        case _ASTL_END_FACET: break; // TODO: check prevState
        case _ASTL_END_FILE:  break; // TODO: check prevState
        case _ASTL_SKIP_LINE: break;
        case _ASTL_BEG_OUTER_LOOP:   
            if (prevState != _ASTL_BEG_FACET) {
                parserTitle = "outer loop is outside facet";
                goto parserError;
            }
            vindex = 0;
            break;
        case _ASTL_VERTEX: {
            GLfloat vdata[3] = { 0 };
            if (!astl_get_vertex(vdata, arg)) {
                parserTitle = "parsing vertex error";
                goto parserError;
            }
            g_array_insert_vals(result->vertex, vindex, vdata, sizeof(vdata));
            //g_array_append_vals(result->index, &vindex, sizeof(vindex));
            vindex += 3;
            ++iindex;
            break;
        }
        case _ASTL_END_OUTER_LOOP:
            if (prevState != _ASTL_VERTEX) {
                parserTitle = "outer loop ends unexpectedly";
                goto parserError;
            }
            break;
        default:
parserError:        
            logBoxTrace(LOGBOX_ERROR, "LOADSTLA [%s(%d)] %s [%d]\n", pathname, lineNum, parserTitle, state);
            readed = 0;
        }
        if (state > _ASTL_SKIP_LINE) {
            prevState = state;
        }
        ++lineNum;
    }
    while (readed > 0);
    logBoxTrace(LOGBOX_ASSERT, "LOADSTLA [%s] [V:%d; I:%d]\n", pathname, result->vertex->len, result->index->len);
    goto noError;
onError:
    logBoxTrace(LOGBOX_ERROR, "LOADSTLA [%s] %s ERROR: [%d] %s\n", 
        pathname,
        errorTitle,
        error ? error->code : errno, 
        error ? error->message : strerror(errno));
    meshFree(result);
    result = NULL;
    if (idata) {
        g_object_unref(idata);
    }
noError:
    return result;
}

PGMesh stlLoadBin(const gchar *pathname, GInputStream *istm)
{
    return NULL;
}