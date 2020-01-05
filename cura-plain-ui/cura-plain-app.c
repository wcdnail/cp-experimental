#include <gtk/gtk.h>

#define _DEF_WND_TITLE  "Cura-Plain UI"
#define _DEF_WND_WIDTH  1024
#define _DEF_WND_HEIGHT 768

static void onAppActivate(GtkApplication* app, gpointer user_data)
{
    GtkWidget *window = gtk_application_window_new(app);
    int         wndCx = _DEF_WND_WIDTH;
    int         wndCy = _DEF_WND_HEIGHT;
    int          wndX = 0;
    int          wndY = 0;
    if (1) {
        GdkDisplay   *defDisp = gdk_display_get_default();
        GdkMonitor    *defMon = gdk_display_get_primary_monitor(defDisp);
        GdkRectangle scrnArea = { 0 };
        gdk_monitor_get_workarea(defMon, &scrnArea);
        wndX = (scrnArea.width - wndCx)/2;
        wndY = (scrnArea.height - wndCy)/2;
    }
    gtk_window_set_title(GTK_WINDOW(window), _DEF_WND_TITLE);
    gtk_window_move(GTK_WINDOW(window), wndX, wndY);
    gtk_window_set_default_size(GTK_WINDOW(window), wndCx, wndCy);
    gtk_widget_show_all(window);
}

int main (int argc, char **argv)
{
    int              rv = 0;
    GtkApplication *app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(onAppActivate), NULL);
    rv = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return rv;
}
