#include <gtk/gtk.h>
#include "cura-plain-settings.h"

static void onAppActivate(GtkApplication* app, gpointer userData)
{
    GtkWidget     *window = gtk_application_window_new(app);
    PAppSettings settings = appSettings();
    gtk_window_set_title(GTK_WINDOW(window), settings->appTitle);
    gtk_window_move(GTK_WINDOW(window), settings->appRect->x, settings->appRect->y);
    gtk_window_set_default_size(GTK_WINDOW(window), settings->appRect->cx, settings->appRect->cy);
    gtk_widget_show_all(window);
}

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
