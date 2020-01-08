#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include "gmodel-view.h"
#include "glog-box.h"
#include "gfile-stl.h"

static PGScene      currentScene = NULL;

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

#if 0
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
#endif

static void gmodel_view_init_startup_scene(void)
{
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
    glClearColor(0.1, 0.39, 0.88, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2, 2, -1.5, 1.5, 1, 40);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -3);
    glRotatef(50, 1, 0, 0);
    glRotatef(70, 0, 1, 0);    
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
}

gboolean gmodelView_OnRender(GMainWin *win)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    glFlush();
    return FALSE;
}
