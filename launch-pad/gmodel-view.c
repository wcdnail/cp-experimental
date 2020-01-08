#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include "gmodel-view.h"
#include "glog-box.h"
#include "gfile-stl.h"

static PGScene currentScene = NULL;

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

void gmodelView_Init(GMainWin *win)
{
    GError *error = NULL;
    gtk_gl_area_make_current(win->modelView);
    if ((error = gtk_gl_area_get_error(win->modelView)) != NULL) {
        logBoxTrace(LOGBOX_ERROR, "MODELVIEW INIT: [%d] %s\n", error->code, error->message);
        return;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.7, 0.8, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2, 2, -1.5, 1.5, 1, 40);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -3);
    glRotatef(50, 1, 0, 0);
    glRotatef(70, 0, 1, 0);

    logBoxTrace(LOGBOX_MSG, "Using OpenGL : %s\n", glGetString(GL_VERSION));
    logBoxTrace(LOGBOX_MSG, "GL vendor    : %s\n", glGetString(GL_VENDOR));
    logBoxTrace(LOGBOX_MSG, "GL renderer  : %s\n", glGetString(GL_RENDERER));
    if (!currentScene) {
        currentScene = gsceneNew(NULL);
    }
    if (currentScene) {
        if (!currentScene->meshes) {
#if 1
            //PGMesh simpleMesh = gmeshNew("CUBE");
#else            
            PGMesh dummyMesh = gresourseLoadSTLA("/wcd/launchpad/model-dummy");
            if (dummyMesh) {
                currentScene->meshes = g_list_insert(currentScene->meshes, dummyMesh, -1);
            }
#endif            
        }
    }
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
    GError *error = NULL;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Draw grid
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
        glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
        glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
    }
    glEnd();
    // Test stuff
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1, 1, 1); glVertex3f(0, 2, 0);
    glColor3f(1, 0, 0); glVertex3f(-1, 0, 1);
    glColor3f(0, 1, 0); glVertex3f(1, 0, 1);
    glColor3f(0, 0, 1); glVertex3f(0, 0, -1.4);
    glColor3f(1, 1, 1); glVertex3f(0, 2, 0);
    glColor3f(1, 0, 0); glVertex3f(-1, 0, 1);
    glEnd();    
    if (currentScene) {
        if (currentScene->meshes) {
            g_list_foreach(currentScene->meshes, gmeshRender, NULL);
        }
    }
    glFlush();
    return FALSE;
}
