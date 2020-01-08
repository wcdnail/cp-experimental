#include <gtk/gtk.h>
#include "gmodel-view.h"
#include "glog-box.h"
#include <GL/gl.h>

void gmodelView_Init(GtkGLArea *ctl)
{
    GError *error = NULL;
    gtk_gl_area_make_current(ctl);
    if ((error = gtk_gl_area_get_error(ctl)) != NULL) {
        logBoxTrace(LOGBOX_ERROR, "GL INIT: [%d] %s\n", error->code, error->message);
        return;
    }
    logBoxTrace(LOGBOX_MSG, "Using OpenGL : %s\n", glGetString(GL_VERSION));
    logBoxTrace(LOGBOX_MSG, "GL vendor    : %s\n", glGetString(GL_VENDOR));
    logBoxTrace(LOGBOX_MSG, "GL renderer  : %s\n", glGetString(GL_RENDERER));
}

void gmodelView_Free(GtkGLArea *ctl)
{
    GError *error = NULL;
    gtk_gl_area_make_current(ctl);
    if ((error = gtk_gl_area_get_error(ctl)) != NULL) {
        logBoxTrace(LOGBOX_ERROR, "GL FREE: [%d] %s\n", error->code, error->message);
        return;
    }
}

gboolean gmodelView_OnRender(GtkGLArea *ctl, gpointer user)
{
    glClearColor(0.7, 0.9, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    return FALSE;
}
