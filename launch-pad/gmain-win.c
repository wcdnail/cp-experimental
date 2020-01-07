#include <gtk/gtk.h>
#include "gmain-win.h"
#include "appconfig.h"
#include "dbg-trace.h"
#include "glog-box.h"

//---------------------------------------------------------------------------------------------------------------------
// GTK Class 

G_DEFINE_TYPE(GMainWin, gmain_win, GTK_TYPE_APPLICATION_WINDOW);

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

static void enum_gsettings_schemas(void) 
{
#ifdef _DEBUG_EXTRA    
    GList                   *schema_list = NULL;
    gchar              **non_relocatable = NULL;
    GSettingsSchemaSource *schema_source = g_settings_schema_source_get_default ();
    g_settings_schema_source_list_schemas(schema_source, TRUE, &non_relocatable, NULL);
    for (gint i = 0; non_relocatable[i] != NULL; i++) {
        schema_list = g_list_prepend(schema_list, non_relocatable[i]);
    }
    for (; schema_list != NULL; schema_list = schema_list->next) {
        GSettingsSchema *schema;
        GSettings    *gsettings;
        schema = g_settings_schema_source_lookup(schema_source, schema_list->data, TRUE);
        g_print("SCHEMA [%s]\n", schema_list->data);
        g_settings_schema_unref(schema);
    }
    g_strfreev(non_relocatable);
    g_list_free(schema_list);
#endif    
}

static void gmain_win_init(GMainWin *win)
{
    GdkPixbuf *mainIcon = NULL;
    GError       *error = NULL;
    gtk_widget_init_template(GTK_WIDGET(win));
    logBoxInit(win->logBox);
    // My settings
    appSettingsOnWindowInit(win);
    // GTK Settings
    enum_gsettings_schemas();
    //win->settings = g_settings_new(_DEF_APP_ID);
    if (win->settings) {
        //g_settings_bind(win->settings, "font", win->logBox, "modify_font", G_SETTINGS_BIND_DEFAULT);
    }
    else {
        g_print("GMAINWIN setup settings ERROR!\n");
    }
    // Appearance
    mainIcon = gdk_pixbuf_new_from_resource("/wcd/launchpad/mainicon", &error);
    if (mainIcon) {
        gtk_window_set_icon(GTK_WINDOW(win), mainIcon);
    }
    else {
        g_print("GMAINWIN setup main icon ERROR: [%d] %s\n", error->code, error->message);
    }
    if (mainIcon) {
        g_object_unref(mainIcon);
    }
    int j = 1;
    for (int i=0; i<10; i++) {
        logBoxTrace(LOGBOX_MSG, "%03d: Plain Message\n", j++);
        logBoxTrace(LOGBOX_NOTE, "%03d: Message\n", j++);
        logBoxTrace(LOGBOX_WARN, "%03d: Warning Message\n", j++);
        logBoxTrace(LOGBOX_ERROR, "%03d: Error Message\n", j++);
    }
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

