#include <gtk/gtk.h>
#include "gmain-win.h"
#include "gsettings.h"

struct _GMainWin
{
    GtkApplicationWindow super;
};

G_DEFINE_TYPE(GMainWin, gmain_win, GTK_TYPE_APPLICATION_WINDOW);

static void gmain_win_dispose(GObject *gobject);

static void gmain_win_class_init(GMainWinClass *cls)
{
    G_OBJECT_CLASS(cls)->dispose = gmain_win_dispose;
}

GMainWin *gmain_win_new(GLaunchPad *app)
{
    return g_object_new(GMAIN_WIN_TYPE, "application", app, NULL);
}

static void gmain_win_init(GMainWin *win)
{
    PAppSettings settings = appSettings();
    gtk_window_set_title(GTK_WINDOW(win), settings->appTitle);
    gtk_window_move(GTK_WINDOW(win), settings->appRect->x, settings->appRect->y);
    gtk_window_set_default_size(GTK_WINDOW(win), settings->appRect->cx, settings->appRect->cy);
}

static void gmain_win_dispose(GObject *gobject)
{
  GMainWin *self = GMAIN_WIN(gobject);
  G_OBJECT_CLASS(gmain_win_parent_class)->dispose(gobject);
}

void gmain_win_open(GMainWin *win, GFile *file)
{
}
