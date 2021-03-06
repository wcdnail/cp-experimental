#include <gtk/gtk.h>
#include "mainwin.h"
#include "mainview.h"
#include "appconfig.h"
#include "logbox.h"

//---------------------------------------------------------------------------------------------------------------------
// GTK Class 

G_DEFINE_TYPE(GMainWin, gmain_win, GTK_TYPE_APPLICATION_WINDOW)

static void mainwin_OnRealize(GMainWin *win, gpointer user);
static void mainwin_OnDispose(GObject *gobject);
static void mainwin_OnUnmap(GMainWin *win, gpointer user);
static void pan_OnPositionNotify(GtkPaned *ctl, GParamSpec *pspec, gpointer user);

static void gmain_win_class_init(GMainWinClass *cls)
{
    G_OBJECT_CLASS(cls)->dispose = mainwin_OnDispose;
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(cls), "/wcd/launchpad/launch-pad.ui");
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, panRoot);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, panView);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, modelView);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, logBox);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, cmdToggleLogScrollDown);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, cmdSingleCommandEditBox);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(cls), GMainWin, cmdRunSingleCommand);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(cls), mainwin_OnRealize);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(cls), mainwin_OnUnmap);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(cls), logBox_OnToggleScrollDown);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(cls), logBox_OnClear);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(cls), modelView_Init);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(cls), modelView_Free);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(cls), modelView_Resize);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(cls), modelView_OnRender);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(cls), modelView_OnEvent);
    gtk_widget_class_bind_template_callback(GTK_WIDGET_CLASS(cls), pan_OnPositionNotify);
}

static void mainwinSettingsInit(GMainWin *win) 
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

#ifdef _DEBUG_LOGBOX
static gboolean test_log_box(gpointer data)
{
    static int j = 1;
    lgTrace(LG_MSG, "%03d: Message message message message message\n", j++);
    lgTrace(LG_NOTE, "%03d: Note Message\n", j++);
    lgTrace(LG_WARN, "%03d: Warning Message message message message\n", j++);
    lgTrace(LG_ERROR, "%03d: Error Message message message message message message\n", j++);
    return TRUE;
}
#endif

static void mainwinModelViewInit(GMainWin *win) 
{
    gtk_widget_set_events(GTK_WIDGET(win->modelView), 0
        | GDK_POINTER_MOTION_MASK
        | GDK_POINTER_MOTION_HINT_MASK
        | GDK_BUTTON_MOTION_MASK
        | GDK_BUTTON1_MOTION_MASK
        | GDK_BUTTON2_MOTION_MASK
        | GDK_BUTTON3_MOTION_MASK
        | GDK_BUTTON_PRESS_MASK
        | GDK_BUTTON_RELEASE_MASK
        | GDK_KEY_PRESS_MASK
        | GDK_KEY_RELEASE_MASK
        | GDK_SCROLL_MASK
        | GDK_TOUCH_MASK
        | GDK_SMOOTH_SCROLL_MASK
        | GDK_TOUCHPAD_GESTURE_MASK
    );
}

static void gmain_win_init(GMainWin *win)
{
    GdkPixbuf *mainIcon = NULL;
    GError       *error = NULL;
    gtk_widget_init_template(GTK_WIDGET(win));
    mainIcon = gdk_pixbuf_new_from_resource("/wcd/launchpad/icon-main", &error);
    if (mainIcon) {
        gtk_window_set_icon(GTK_WINDOW(win), mainIcon);
    }
    else {
        g_print("GMAINWIN setup main icon ERROR: [%d] %s\n", error->code, error->message);
    }
    if (mainIcon) {
        g_object_unref(mainIcon);
    }
}

//---------------------------------------------------------------------------------------------------------------------

GMainWin *mainWinNew(GLaunchPad *app)
{
    return g_object_new(GMAIN_WIN_TYPE, "application", app, NULL);
}

void mainWinOpen(GMainWin *win, GFile *file)
{
}

static void mainwin_OnRealize(GMainWin *win, gpointer user)
{
    logBoxInit(win->logBox);
    mainwinModelViewInit(win);
    // Command line
    gtk_widget_set_sensitive(GTK_WIDGET(win->cmdSingleCommandEditBox), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(win->cmdRunSingleCommand), FALSE);
    // My settings
    appSettingsOnWindowInit(win);
    // gLib Settings
    mainwinSettingsInit(win);
    // Appearance
#ifdef _DEBUG_LOGBOX
    g_timeout_add_seconds(1, test_log_box, NULL);
#endif
}

static void mainwin_OnDispose(GObject *gobject)
{
    GMainWin *win = GMAIN_WIN(gobject);
    G_OBJECT_CLASS(gmain_win_parent_class)->dispose(gobject);
}

static void mainwin_OnUnmap(GMainWin *win, gpointer user)
{
    appSettingsOnWindowClose(win);
}

static void pan_OnPositionNotify(GtkPaned *ctl, GParamSpec *pspec, gpointer user)
{
    GMainWin    *win = GMAIN_WIN(user);
    gint      panpos = gtk_paned_get_position(ctl);
    gboolean rootPan = ctl == win->panRoot;
    gint          cx = 0;
    gint          cy = 0;
    gdk_window_get_geometry(gtk_widget_get_window(GTK_WIDGET(win)), NULL, NULL, &cx, &cy);
    if (rootPan) {
        gtk_paned_set_position(win->panRoot, cy - appSettings()->logPanelCy);
    }
    else {
        gtk_paned_set_position(win->panView, cx - appSettings()->confPanelCx);
    }
}
