#include <gtk/gtk.h>
#include "appconfig.h"
#include "glaunch-pad.h"
#include "dbg-trace.h"
#include "glog-box.h"
#include <glib-object.h>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

//---------------------------------------------------------------------------------------------------------------------

static PAppSettings     currentSettings = NULL;
static GString *currentSettingsPathname = NULL;

//---------------------------------------------------------------------------------------------------------------------

#define _DEF_WND_TITLE      "LaunchPad 1.0"
#define _DEF_WND_WIDTH      1024
#define _DEF_WND_HEIGHT     768
#define _DEF_PAN_ROOT_CY    300
#define _DEF_PAN_VIEW_CX    220

//---------------------------------------------------------------------------------------------------------------------

struct _AppSettingsClass
{
    GObjectClass super;
};

typedef struct _AppSettingsClass AppSettingsClass;

#define APPSETTINGS_TYPE_OBJECT  (app_settings_get_type())
#define APPSETTINGS_OBJECT(obj)  (G_TYPE_CHECK_INSTANCE_CAST((obj), APPSETTINGS_TYPE_OBJECT, AppSettings))

G_DEFINE_TYPE(AppSettings, app_settings, G_TYPE_OBJECT);

//---------------------------------------------------------------------------------------------------------------------

static void app_settings_set_property(GObject *gobject, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    PAppSettings self = APPSETTINGS_OBJECT(gobject);
    switch (prop_id)
    {
    case PROP_APPSETTING_ID:             self->id = g_value_get_uint(value); break;
    case PROP_APPSETTING_RECT:           grect_set_rect(self->appRect, (PGRect)g_value_get_object(value)); break;
    case PROP_APPSETTING_ISMAX:          self->appIsMaximized = g_value_get_boolean(value); break;
    case PROP_APPSETTING_LOG_CY:   self->logPanelCy = g_value_get_int(value); break;
    case PROP_APPSETTING_CONF_CX:   self->confPanelCx = g_value_get_int(value); break;
    case PROP_APPSETTING_TITLE: {
        g_free((gpointer)self->appTitle);
        self->appTitle = g_value_dup_string(value);
        break;
    }
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);
    }
}

static void app_settings_get_property(GObject *gobject, guint prop_id, GValue *value, GParamSpec *pspec)
{
    PAppSettings self = APPSETTINGS_OBJECT(gobject);
    switch (prop_id)
    {
    case PROP_APPSETTING_ID:             g_value_set_uint(value, self->id); break;
    case PROP_APPSETTING_RECT:           g_value_set_object(value, self->appRect); break;
    case PROP_APPSETTING_ISMAX:          g_value_set_boolean(value, self->appIsMaximized); break;
    case PROP_APPSETTING_LOG_CY:   g_value_set_int(value, self->logPanelCy); break;
    case PROP_APPSETTING_CONF_CX:   g_value_set_int(value, self->confPanelCx); break;
    case PROP_APPSETTING_TITLE:          g_value_set_string(value, self->appTitle); break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);
    }
}

static void app_settings_finalize(GObject *gobject)
{
    PAppSettings self = APPSETTINGS_OBJECT(gobject);
    g_object_unref(self->appRect);
    g_free((gpointer)self->appTitle);
    G_OBJECT_CLASS(app_settings_parent_class)->finalize(gobject);
}

static void app_settings_class_init(AppSettingsClass *cls)
{
    GObjectClass *gobject = G_OBJECT_CLASS(cls);
    gobject->set_property = app_settings_set_property;
    gobject->get_property = app_settings_get_property;
    gobject->finalize = app_settings_finalize;
    g_object_class_install_property(gobject, PROP_APPSETTING_ID, g_param_spec_uint("id", "Id", "", 0, G_MAXUINT, 0, G_PARAM_READWRITE));
    g_object_class_install_property(gobject, PROP_APPSETTING_TITLE, g_param_spec_string("appTitle", "AppTitle", "", NULL, G_PARAM_READWRITE));
    g_object_class_install_property(gobject, PROP_APPSETTING_RECT, g_param_spec_object("appRect", "AppRect", "", grect_get_type(), G_PARAM_READWRITE));
    g_object_class_install_property(gobject, PROP_APPSETTING_ISMAX, g_param_spec_boolean("appIsMaximized", "AppIsMaximized", "", FALSE, G_PARAM_READWRITE));
    g_object_class_install_property(gobject, PROP_APPSETTING_LOG_CY, g_param_spec_int("logPanelCy", "LogPanelCy", "", 0, G_MAXINT, 0, G_PARAM_READWRITE));
    g_object_class_install_property(gobject, PROP_APPSETTING_CONF_CX, g_param_spec_int("confPanelCx", "ConfigPanelCx", "", 0, G_MAXINT, 0, G_PARAM_READWRITE));
}

static void app_settings_init(PAppSettings self)
{
    self->id = 0;
    self->appTitle = g_strdup(_DEF_WND_TITLE);
    self->appRect = grect_new_size(_DEF_WND_WIDTH, _DEF_WND_HEIGHT);
    self->appIsMaximized = FALSE;
    self->logPanelCy = _DEF_PAN_ROOT_CY;
    self->confPanelCx = _DEF_PAN_VIEW_CX;
    grect_put_to_center_of_screen(self->appRect);
}

//---------------------------------------------------------------------------------------------------------------------

#define _DEF_APPSETTINGS_PATHNAME _DEF_APP_ID "-settings.json"

static void app_settings_free_current_pathname(void) 
{
    if (currentSettingsPathname) {
        g_string_free(currentSettingsPathname, TRUE);
        currentSettingsPathname = NULL;
    }
}

static gboolean app_settings_init_current_pathname(void) 
{
    if (!currentSettingsPathname) {
        currentSettingsPathname = g_string_new(g_get_user_config_dir());
        if (currentSettingsPathname) {
            g_string_append(currentSettingsPathname, "/" _DEF_APP_ID);
            if (-1 == g_mkdir_with_parents(currentSettingsPathname->str, 0755)) {
                app_settings_free_current_pathname();
                return (FALSE);
            }
            g_string_append(currentSettingsPathname, "/current-settings.json");
        }
    }
    return (NULL != currentSettingsPathname);
}

PAppSettings appSettings(void)
{
    const gchar *errorTitle = NULL;
    GError           *error = NULL;
    gchar          *jsonstr = NULL;
    if (!currentSettings) {
        gsize readed = -1;
        if (!app_settings_init_current_pathname()) {
            errorTitle = "pathname init ERROR";
            goto onError;
        }
        if (g_file_get_contents(currentSettingsPathname->str, &jsonstr, &readed, &error)) {
            currentSettings = (PAppSettings)json_gobject_from_data(APPSETTINGS_TYPE_OBJECT, jsonstr, readed, &error);
            if (!currentSettings) {
                errorTitle = "deserializing object from json FAILURE";
                goto onError;
            }
        }
        if (!currentSettings) {
            errorTitle = "loading ERROR";
            goto onError;
        }
    }
    goto noError;
onError:
    if (error) {
        g_print("  SETTINGS [%s] %s: [%d] %s. Using defaults...\n", 
            currentSettingsPathname->str, 
            errorTitle ? errorTitle : "unknown",
            error->code, 
            error->message);
    }
    else {
      g_print("  SETTINGS [%s] %s! Using defaults...\n", 
            currentSettingsPathname->str, 
            errorTitle ? errorTitle : "unknown");
    }
    if (jsonstr) {
        g_free(jsonstr);
    }
    currentSettings = (PAppSettings)g_object_new(APPSETTINGS_TYPE_OBJECT, NULL);
noError:        
    return currentSettings;
}

void appSettingsFree(void)
{
    app_settings_free_current_pathname();
    if (currentSettings) {
        g_object_unref(currentSettings);
    }
    currentSettings = NULL;
}

void appSettingsSave(void)
{
    const char *errorTitle = NULL;
    GError          *error = NULL;
    gsize  settingsJsonLen = 0;
    gchar    *settingsJson = NULL;
    if (!currentSettings) {
        errorTitle = "object is NULL";
        goto onError;
    }
    settingsJson = json_gobject_to_data(G_OBJECT(currentSettings), &settingsJsonLen);
    if (!settingsJson) {
        errorTitle = "json serialization failed";
        goto onError;
    }
#ifdef _DEBUG
    g_print("  SETTINGS [%s]:\n%s\n", currentSettingsPathname->str, settingsJson);
#endif
    if (!app_settings_init_current_pathname()) {
        errorTitle = "pathname init ERROR";
        goto onError;
    }
    if (!g_file_set_contents(currentSettingsPathname->str, settingsJson, settingsJsonLen, &error)) {
        goto onError;    
    }
    goto noError;
onError:
    if (error) {
        g_print("  SETTINGS [%s] %s: [%d] %s\n", 
            currentSettingsPathname->str, 
            errorTitle,
            error->code, 
            error->message);
    }
    else {
        g_print("  SETTINGS [%s] %s!\n", 
            currentSettingsPathname->str, 
            errorTitle);
    }
noError:       
    if (settingsJson) {
        g_free(settingsJson);
    }
}

void appSettingsOnWindowInit(GMainWin *win)
{
    PAppSettings settings = appSettings();
    gtk_window_set_title(GTK_WINDOW(win), settings->appTitle);
    gtk_window_set_default_size(GTK_WINDOW(win), settings->appRect->cx, settings->appRect->cy);
    gtk_window_move(GTK_WINDOW(win), settings->appRect->x, settings->appRect->y);
    if (settings->appIsMaximized) {
        gtk_window_maximize(GTK_WINDOW(win));
    }
    gtk_toggle_tool_button_set_active(win->logctlToggleScrollDown, TRUE);
}

void appSettingsOnWindowClose(GMainWin *win)
{
    PAppSettings settings = appSettings();
    GRect          rcTemp = { 0 };
    settings->appIsMaximized = gtk_window_is_maximized(GTK_WINDOW(win));
    if (!settings->appIsMaximized) {
        gdk_window_get_geometry(gtk_widget_get_window(GTK_WINDOW(win)), &rcTemp.x, &rcTemp.y, &rcTemp.cx, &rcTemp.cy);
        grect_set_rect(settings->appRect, &rcTemp);
    }
}
