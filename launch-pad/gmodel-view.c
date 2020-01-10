#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include "gmodel-view.h"
#include "glog-box.h"
#include "gfile-stl.h"

static PGScene currentScene = NULL;

PGScene sceneNew(GList *meshes)
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
    sceneFree(resScene);
    resScene = NULL;
noError:
    return resScene;
}

void sceneFree(PGScene scene)
{
    if (scene) {
        g_list_foreach(scene->meshes, gmeshFree, NULL);
        g_list_free(scene->meshes);
        scene->meshes = NULL;
        g_free(scene);
    }
}

#if 0
static guint model_view_create_shader(int shader_type, const char *source)
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

static void model_view_init_shaders(void)
{
    GBytes     *source = NULL;
    int         status = 0;
    guint vertexShader = 0;
    guint   fragShader = 0;
    source = g_resources_lookup_data("/wcd/launchpad/def-shader-vertex", 0, NULL);
    vertexShader = model_view_create_shader(GL_VERTEX_SHADER, g_bytes_get_data(source, NULL));
    g_bytes_unref(source);
    source = g_resources_lookup_data("/wcd/launchpad/def-shader-fragment", 0, NULL);
    fragShader = model_view_create_shader(GL_FRAGMENT_SHADER, g_bytes_get_data(source, NULL));
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

static void model_view_init_startup_scene(void)
{
    if (!currentScene) {
        currentScene = sceneNew(NULL);
    }
    if (currentScene) {
        if (!currentScene->meshes) {
            PGMesh dummyMesh = stlLoadResource("/wcd/launchpad/model-dummy");
            if (dummyMesh) {
                currentScene->meshes = g_list_insert(currentScene->meshes, dummyMesh, -1);
            }
        }
    }
}

void modelView_Init(GtkGLArea *ctl)
{
    GError *error = NULL;
    gtk_gl_area_make_current(ctl);
    if ((error = gtk_gl_area_get_error(ctl)) != NULL) {
        logBoxTrace(LOGBOX_ERROR, "MODELVIEW init ERROR: [%d] %s\n", error->code, error->message);
        return;
    }
    logBoxTrace(LOGBOX_MSG, "Using OpenGL : %s\n", glGetString(GL_VERSION));
    logBoxTrace(LOGBOX_MSG, "GL vendor    : %s\n", glGetString(GL_VENDOR));
    logBoxTrace(LOGBOX_MSG, "GL renderer  : %s\n", glGetString(GL_RENDERER));

    glClearColor(1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -3);
    glRotatef(50, 1, 0, 0);
    glRotatef(70, 0, 1, 0);

    model_view_init_startup_scene();
}

void modelView_Free(GtkGLArea *ctl)
{
    GError *error = NULL;
    sceneFree(currentScene);
    currentScene = NULL;
    gtk_gl_area_make_current(ctl);
    if ((error = gtk_gl_area_get_error(ctl)) != NULL) {
        logBoxTrace(LOGBOX_ERROR, "GL FREE: [%d] %s\n", error->code, error->message);
        return;
    }
}

void modelView_Resize(GtkGLArea *ctl, gint cx, gint cy)
{
    GLfloat aspect = cy ? (GLfloat)cx / (GLfloat)cy : (GLfloat)cx;
    if (0) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-2.5, 2.5, -2.5/aspect, 2.5/aspect, 1, 10.0);
    }
    else {
        glViewport(0, 0, cx, cy);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (cx <= cy) {
            glOrtho(-2.5, 2.5, -2.5/aspect, 2.5/aspect, -10.0, 10.0);
        } 
        else {
            glOrtho(-2.5*aspect, 2.5*aspect, -2.5, 2.5, -10.0, 10.0);
        }
    }
}

gboolean modelView_OnEvent(GtkGLArea *ctl, GdkEvent *event, gpointer user)
{
    logBoxTrace(LOGBOX_NOTE, "MVEVENT: [%d]\n", event->type);
    return (FALSE);
}

gboolean modelView_OnRender(GtkGLArea *ctl, GdkGLContext *context)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Draw grid
    glColor4f(0, 0, 0, 1);
    glBegin(GL_LINES);
    for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
        glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
        glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
    }
    glEnd();
#if 1
    // Test stuff
    glBegin(GL_TRIANGLE_STRIP);
    glColor4f(1, 0, 1, 1); glVertex3f(0, 2, 0);
    glColor4f(1, 0, 0, 1); glVertex3f(-1, 0, 1);
    glColor4f(0, 1, 0, 1); glVertex3f(1, 0, 1);
    glColor4f(0, 0, 1, 1); glVertex3f(0, 0, -1.4);
    glColor4f(1, 1, 0, 1); glVertex3f(0, 2, 0);
    glColor4f(1, 0, 0, 1); glVertex3f(-1, 0, 1);
    glEnd();
#endif    
    if (currentScene) {
        if (currentScene->meshes) {
            g_list_foreach(currentScene->meshes, gmeshRender, NULL);
        }
    }
    glFlush();
    return (FALSE);
}
