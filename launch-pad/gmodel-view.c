#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include "gmodel-view.h"
#include "glog-box.h"
#include "gfile-stl.h"

static PGScene      currentScene = NULL;
static guint        defGLProgram = 0;
static guint      defMvpLocation = 0;
static guint defPositionLocation = 0;
static guint    defColorLocation = 0;
static guint      defVertexArray = 0;
static float          defMvp[16] = { 0 };

static void gmodel_view_init_mvp(float *res) 
{
    res[0] = 1.f; res[4] = 0.f;  res[8] = 0.f; res[12] = 0.f;
    res[1] = 0.f; res[5] = 1.f;  res[9] = 0.f; res[13] = 0.f;
    res[2] = 0.f; res[6] = 0.f; res[10] = 1.f; res[14] = 0.f;
    res[3] = 0.f; res[7] = 0.f; res[11] = 0.f; res[15] = 1.f;
}

PGScene gsceneNew(GList *meshes)
{
    const gchar *errorTitle = NULL;
    PGScene        resScene = NULL; 
    resScene = g_malloc(sizeof(GMesh));
    if (!resScene) {
        errorTitle = "scene allocation";
        goto onError;
    }
    resScene->meshes = meshes ? g_list_copy(meshes) : NULL;
    if (meshes && !resScene->meshes) {
        errorTitle = "mesh list copy";
        goto onError;
    }
    goto noError;
onError:
    logBoxTrace(LOGBOX_ERROR, "MODELVEW %s ERROR: [%d] %s\n", errorTitle, errno, strerror(errno));
    gsceneFree(resScene);
    resScene = NULL;
noError:
    return resScene;
}

void gsceneFree(PGScene scene)
{
    if (scene) {
        g_list_foreach(scene->meshes, gmeshFree, NULL);
        g_list_free(scene->meshes);
        scene->meshes = NULL;
        g_free(scene);
    }
}

static guint gmodel_view_create_shader(int shader_type, const char *source)
{
    int   status = 0;
    guint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        char *buffer = NULL;
        int  log_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
        buffer = g_malloc(log_len + 1);
        glGetShaderInfoLog(shader, log_len, NULL, buffer);
        logBoxTrace(LOGBOX_ERROR, "MODELVIEW load shader ERROR: [%d] %s\n", status, buffer);
        g_free(buffer);
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}

static void gmodel_view_init_shaders(void)
{
    GBytes     *source = NULL;
    int         status = 0;
    guint vertexShader = 0;
    guint   fragShader = 0;
    source = g_resources_lookup_data("/wcd/launchpad/def-shader-vertex", 0, NULL);
    vertexShader = gmodel_view_create_shader(GL_VERTEX_SHADER, g_bytes_get_data(source, NULL));
    g_bytes_unref(source);
    source = g_resources_lookup_data("/wcd/launchpad/def-shader-fragment", 0, NULL);
    fragShader = gmodel_view_create_shader(GL_FRAGMENT_SHADER, g_bytes_get_data(source, NULL));
    g_bytes_unref(source);
    if (vertexShader || fragShader) {
        defGLProgram = glCreateProgram();
        glAttachShader(defGLProgram, vertexShader);
        glAttachShader(defGLProgram, fragShader);
        glLinkProgram(defGLProgram);
        glGetProgramiv(defGLProgram, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            char *buffer = NULL;
            int  log_len = 0;
            glGetProgramiv(defGLProgram, GL_INFO_LOG_LENGTH, &log_len);
            buffer = g_malloc(log_len + 1);
            glGetProgramInfoLog(defGLProgram, log_len, NULL, buffer);
            logBoxTrace(LOGBOX_ERROR, "MODELVIEW link GL program ERROR: [%d] %s\n", status, buffer);
            g_free (buffer);
            glDeleteProgram (defGLProgram);
            defGLProgram = 0;
        }
        else {
            defMvpLocation = glGetUniformLocation(defGLProgram, "mvp");
            defPositionLocation = glGetAttribLocation (defGLProgram, "position");
            defColorLocation = glGetAttribLocation (defGLProgram, "color");
            glDetachShader(defGLProgram, vertexShader);
            glDetachShader(defGLProgram, fragShader);
        }
    }
    if (vertexShader) {
        glDeleteShader(vertexShader);
    }
    if (fragShader) {
        glDeleteShader(fragShader);
    }
}

static void gmodel_view_init_startup_scene(void)
{
    static const GVertex vertData[] = {
        {  0.0f,  0.500f, 0.0f },
        {  0.5f, -0.366f, 0.0f },
        { -0.5f, -0.366f, 0.0f },
    };
    guint buffer = 0;
    glGenVertexArrays(1, &defVertexArray);
    glBindVertexArray(defVertexArray);
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), vertData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(defPositionLocation);
    glVertexAttribPointer(defPositionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(struct _GVertex), (GLvoid*)(G_STRUCT_OFFSET(struct _GVertex, pos)));
    //glEnableVertexAttribArray(color_index);
    //glVertexAttribPointer(color_index, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_info), (GLvoid*) (G_STRUCT_OFFSET(struct vertex_info, color)));
    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &buffer);
    if (!currentScene) {
        currentScene = gsceneNew(NULL);
    }
    if (currentScene) {
        if (!currentScene->meshes) {
            PGMesh dummyMesh = gresourseLoadSTLA("/wcd/launchpad/model-dummy");
            if (dummyMesh) {
                currentScene->meshes = g_list_insert(currentScene->meshes, dummyMesh, -1);
            }
        }
    }
}

void gmodelView_Init(GMainWin *win)
{
    GError *error = NULL;
    gtk_gl_area_make_current(win->modelView);
    if ((error = gtk_gl_area_get_error(win->modelView)) != NULL) {
        logBoxTrace(LOGBOX_ERROR, "MODELVIEW init ERROR: [%d] %s\n", error->code, error->message);
        return;
    }
    logBoxTrace(LOGBOX_MSG, "Using OpenGL : %s\n", glGetString(GL_VERSION));
    logBoxTrace(LOGBOX_MSG, "GL vendor    : %s\n", glGetString(GL_VENDOR));
    logBoxTrace(LOGBOX_MSG, "GL renderer  : %s\n", glGetString(GL_RENDERER));
    gmodel_view_init_mvp(defMvp);
    gmodel_view_init_shaders();
    gmodel_view_init_startup_scene();
}

void gmodelView_Free(GMainWin *win)
{
    GError *error = NULL;
    gsceneFree(currentScene);
    currentScene = NULL;
    gtk_gl_area_make_current(win->modelView);
    if ((error = gtk_gl_area_get_error(win->modelView)) != NULL) {
        logBoxTrace(LOGBOX_ERROR, "GL FREE: [%d] %s\n", error->code, error->message);
        return;
    }
    if (defVertexArray) {
        glDeleteVertexArrays(1, &defVertexArray);
        defVertexArray = 0;
    }
    if (defGLProgram) {
        glDeleteProgram(defGLProgram);
        defGLProgram = 0;
    }
}

gboolean gmodelView_OnRender(GMainWin *win)
{
    glClearColor (0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(defGLProgram);
    glUniformMatrix4fv(defMvpLocation, 1, GL_FALSE, &(defMvp[0]));
    glBindVertexArray(defVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);
    //glColor4f(1, 0, 0, 1);
    //glBegin(GL_LINES);
    //for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
    //    glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
    //    glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
    //}
    //glEnd();
#if 0    
    // Draw grid
    glColor4f(1, 0, 0, 1);
    glBegin(GL_LINES);
    for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
        glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
        glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
    }
    glEnd();
    // Test stuff
    glBegin(GL_TRIANGLE_STRIP);
    glColor4f(1, 1, 1, 1); glVertex3f(0, 2, 0);
    glColor4f(1, 0, 0, 1); glVertex3f(-1, 0, 1);
    glColor4f(0, 1, 0, 1); glVertex3f(1, 0, 1);
    glColor4f(0, 0, 1, 1); glVertex3f(0, 0, -1.4);
    glColor4f(1, 1, 1, 1); glVertex3f(0, 2, 0);
    glColor4f(1, 0, 0, 1); glVertex3f(-1, 0, 1);
    glEnd();    
    if (currentScene) {
        if (currentScene->meshes) {
            g_list_foreach(currentScene->meshes, gmeshRender, NULL);
        }
    }
#endif    
    glFlush();
    return FALSE;
}
