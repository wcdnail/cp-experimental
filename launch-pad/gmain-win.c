#include <gtk/gtk.h>
#include "gmain-win.h"
#include "appconfig.h"
#include "dbg-trace.h"

//---------------------------------------------------------------------------------------------------------------------

G_DEFINE_TYPE(GMainWin, gmain_win, GTK_TYPE_APPLICATION_WINDOW);

//---------------------------------------------------------------------------------------------------------------------

static void gmain_win_dispose(GObject *gobject);
static void gmain_win_on_map(GMainWin *win);
static void gmain_win_on_unmap(GMainWin *win);

static void gmain_win_class_init(GMainWinClass *cls)
{
    G_OBJECT_CLASS(cls)->dispose = gmain_win_dispose;
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(cls), "/wcd/launchpad/launch-pad.ui");
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, panRoot);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, panView);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, modelView);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, configTree);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, configTreeSel);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, logBox);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(cls), gmain_win_on_map);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(cls), gmain_win_on_unmap);
}

GMainWin *gmain_win_new(GLaunchPad *app)
{
    return g_object_new(GMAIN_WIN_TYPE, "application", app, NULL);
}

//---------------------------------------------------------------------------------------------------------------------

static void gmain_win_init(GMainWin *win)
{
    gtk_widget_init_template(GTK_WIDGET(win));
    appSettingsOnWindowInit(win);
    //win->settings = g_settings_new(_DEF_APP_ID);
    //g_settings_bind(win->settings, "transition", win->stack, "transition-type", G_SETTINGS_BIND_DEFAULT);    
}

void gmain_win_open(GMainWin *win, GFile *file)
{
}

static void gmain_win_dispose(GObject *gobject)
{
    GMainWin *win = GMAIN_WIN(gobject);
    g_clear_object(&win->settings);
    G_OBJECT_CLASS(gmain_win_parent_class)->dispose(gobject);
}

static void gmain_win_on_map(GMainWin *win)
{
}

static void gmain_win_on_unmap(GMainWin *win)
{
    appSettingsOnWindowClose(win);
}

