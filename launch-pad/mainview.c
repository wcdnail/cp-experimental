#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include "mainview.h"
#include "logbox.h"
#include "loadstl.h"

static PGScene currentScene = NULL;

PGScene sceneNew(GList *meshes)
{
    const gchar *errorTitle = NULL;
    PGScene           scene = NULL; 
    scene = g_malloc(sizeof(GScene));
    if (!scene) {
        errorTitle = "scene allocation";
        goto onError;
    }
    scene->meshes = meshes ? g_list_copy(meshes) : NULL;
    if (meshes && !scene->meshes) {
        errorTitle = "mesh list copy";
        goto onError;
    }
    scene->isPerspectiveView = TRUE;
    sceneSetPosition(scene, 0, 0, 0);
    sceneSetRotation(scene, 0, 0, 0);
    scene->isRotating = FALSE;
    scene->lx = 0;
    scene->ly = 0;
    goto noError;
onError:
    logBoxTrace(LOGBOX_ERROR, "MODELVEW %s ERROR: [%d] %s\n", errorTitle, errno, strerror(errno));
    sceneFree(scene);
    scene = NULL;
noError:
    return scene;
}

void sceneFree(PGScene scene)
{
    if (scene) {
        g_list_foreach(scene->meshes, meshFree, NULL);
        g_list_free(scene->meshes);
        scene->meshes = NULL;
        g_free(scene);
    }
}

static void model_view_init_startup_scene(void)
{
    if (!currentScene) {
        currentScene = sceneNew(NULL);
    }
    if (currentScene) {
        sceneSetPosition(currentScene, 0, 0, -5);
        sceneSetRotation(currentScene, 45, 45, 0);
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
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
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
    gdouble aspect = cy ? (gdouble)cx / (gdouble)cy : 1;
    glViewport(0, 0, cx, cy);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (currentScene && currentScene->isPerspectiveView) {
        glFrustum(-1, 1, -1/aspect, 1/aspect, 1, 300);
    }
    else {
        if (cx <= cy) {
            glOrtho(-1, 1, -1/aspect, 1/aspect, 1, 300);
        } 
        else {
            glOrtho(-1*aspect, 1*aspect, -1, 1, 1, 300);
        }
    }
}

gboolean modelView_OnEvent(GtkGLArea *ctl, GdkEvent *event, gpointer user)
{
    gdouble gx, gy, dx, dy;
    switch (event->type) {
    case GDK_SCROLL:
        if (currentScene) {
            gdk_event_get_coords(event, &gx, &gy);
            gdk_event_get_scroll_deltas(event, &dx, &dy);
            currentScene->position.z -= dy * 0.5;
            gtk_widget_queue_draw(GTK_WIDGET(ctl));
        }
        break;
    case GDK_BUTTON_RELEASE:
        if (currentScene) {
            currentScene->isRotating = FALSE;
        }
        break;
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
        if (currentScene) {
            gint btn;
            gdk_event_get_button(event, &btn);
            gdk_event_get_coords(event, &gx, &gy);
            currentScene->isRotating = btn == 3;
            currentScene->lx = gx;
            currentScene->ly = gy;
        }
        break;
    case GDK_MOTION_NOTIFY: 
        if (currentScene) {
            gdk_event_get_coords(event, &gx, &gy);
            if(currentScene->isRotating) {
                dx = gx - currentScene->lx;
                dy = currentScene->ly - gy;
                currentScene->lx = gx;
                currentScene->ly = gy;
                currentScene->rotation.x -= dy * 0.3;
                currentScene->rotation.y += dx * 0.3;
                gtk_widget_queue_draw(GTK_WIDGET(ctl));
            }
        }
        break;
    }
    return (FALSE);
}

gboolean modelView_OnRender(GtkGLArea *ctl, GdkGLContext *context)
{
    PGVertex position;
    PGVertex rotation;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (currentScene) {
        position = &currentScene->position;
        rotation = &currentScene->rotation;
        // Position & rotation
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslated(position->x, position->y, position->z);
        glRotated(rotation->x, 1, 0, 0);
        glRotated(rotation->y, 0, 1, 0);
        glRotated(rotation->z, 0, 1, 0);
        // Draw grid
        glColor4f(0, 0, 0, 1);
        glBegin(GL_LINES);
        for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
            glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
            glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
        }
        glEnd();
        if (currentScene->meshes) {
            g_list_foreach(currentScene->meshes, meshRender, NULL);
        }
    }
    glFlush();
    return (FALSE);
}
