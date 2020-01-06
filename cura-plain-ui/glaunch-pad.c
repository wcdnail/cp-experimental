#include <gtk/gtk.h>
#include "glaunch-pad.h"
#include "gmain-win.h"

/*
int main (int argc, char **argv)
{
    int              rv = 0;
    GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(onAppActivate), NULL);
    rv = g_application_run(G_APPLICATION(app), argc, argv);
    appSettingSave();
    appSettingsFree();
    g_object_unref(app);
    return rv;
}
*/

struct _GLaunchPad
{
    GtkApplication super;
};

G_DEFINE_TYPE(GLaunchPad, glaunch_pad, GTK_TYPE_APPLICATION);

static void glaunch_pad_init(GLaunchPad *app)
{
}

static void glaunch_pad_activate(GApplication *app)
{
    GMainWin *win;
    win = gmain_win_new(GLAUNCH_PAD(app));
    gtk_window_present(GTK_WINDOW(win));
}

static void glaunch_pad_open(GApplication *app, GFile **files, gint n_files, const gchar *hint)
{
    GList *windows = NULL;
    GMainWin  *win = NULL;
    windows = gtk_application_get_windows(GTK_APPLICATION(app));
    if (windows) {
        win = GMAIN_WIN(windows->data);
    }
    else {
        win = gmain_win_new(GLAUNCH_PAD(app));
    }
    for (int i = 0; i < n_files; i++) {
        gmain_win_open(win, files[i]);
    }
    gtk_window_present(GTK_WINDOW(win));
}

static void glaunch_pad_class_init(GLaunchPadClass *cls)
{
    G_APPLICATION_CLASS(cls)->activate = glaunch_pad_activate;
    G_APPLICATION_CLASS(cls)->open = glaunch_pad_open;
}

GLaunchPad *glaunch_pad_new(void)
{
    return g_object_new(GLAUNCH_PAD_TYPE, "application-id", "org.gtk.exampleapp", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
