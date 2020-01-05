#include <gtk/gtk.h>
#include "cura-plain-settings.h"

static void onAppActivate(GtkApplication* app, gpointer userData)
{
    GtkWidget         *window = gtk_application_window_new(app);
    PCP_APP_SETTINGS settings = initAppSettings();
    gtk_window_set_title(GTK_WINDOW(window), settings->mainAppTitle);
    gtk_window_move(GTK_WINDOW(window), settings->rcMainApp.x, settings->rcMainApp.y);
    gtk_window_set_default_size(GTK_WINDOW(window), settings->rcMainApp.width, settings->rcMainApp.height);
    gtk_widget_show_all(window);
}

int main (int argc, char **argv)
{
    int                    rv = 0;
    GtkApplication       *app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(onAppActivate), NULL);
    rv = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return rv;
}
