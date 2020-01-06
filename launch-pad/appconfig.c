#include <gtk/gtk.h>
#include "appconfig.h"
#include "filemanip.h"
#include "glaunch-pad.h"
#include <glib-object.h>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

//---------------------------------------------------------------------------------------------------------------------

#define _DEF_APPSETTINGS_PATHNAME _DEF_APP_ID "-settings.json"

#define _DEF_WND_TITLE  "Cura-Plain UI"
#define _DEF_WND_WIDTH  1024
#define _DEF_WND_HEIGHT 768

//---------------------------------------------------------------------------------------------------------------------

struct _AppSettingsClass
{
    GObjectClass super;
};

typedef struct _AppSettingsClass AppSettingsClass;

#define APPSETTINGS_TYPE_OBJECT                (app_settings_get_type())
#define APPSETTINGS_OBJECT(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), APPSETTINGS_TYPE_OBJECT, AppSettings))

G_DEFINE_TYPE(AppSettings, app_settings, G_TYPE_OBJECT);

static void app_settings_set_property(GObject *gobject, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    PAppSettings self = APPSETTINGS_OBJECT(gobject);
    switch (prop_id)
    {
    case PROP_APPSETTING_ID:             self->id = g_value_get_uint(value); break;
    case PROP_APPSETTING_RECT:           grect_set_rect(self->appRect, (PGRect)g_value_get_object(value)); break;
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
}

static void app_settings_init(PAppSettings self)
{
    self->id = 0;
    self->appTitle = g_strdup(_DEF_WND_TITLE);
    self->appRect = grect_new_size(_DEF_WND_WIDTH, _DEF_WND_HEIGHT);
    grect_put_to_center_of_screen(self->appRect);
}

//---------------------------------------------------------------------------------------------------------------------

static PAppSettings currentSettings = NULL;

PAppSettings appSettings(void)
{
    static const char pathname[] = _DEF_APPSETTINGS_PATHNAME;
    const char       *errorTitle = NULL;
    GError                *error = NULL;
    guchar              *jsonstr = NULL;
    if (!currentSettings) {
        gssize readed = -1;
        jsonstr = fileRead(pathname, &readed);
        if(jsonstr) {
            currentSettings = (PAppSettings)json_gobject_from_data(APPSETTINGS_TYPE_OBJECT, (gchar*)jsonstr, readed, &error);
            if (!currentSettings) {
                errorTitle = "object from json";
                goto onError;
            }
        }
        if (!currentSettings) {
            errorTitle = "loading";
            goto onError;
        }
    }
    goto noError;
onError:
    if (error) {
        g_print("SETTINGS [%s] %s ERROR: [%d] %s. Using defaults...\n", 
            pathname, 
            errorTitle ? errorTitle : "unknown",
            error->code, 
            error->message);
    }
    else {
      g_print("SETTINGS [%s] %s ERROR! Using defaults...\n", 
            pathname, 
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
    if (currentSettings) {
        g_object_unref(currentSettings);
    }
    currentSettings = NULL;
}

void appSettingsSave(void)
{
    gsize settingsJsonLen = 0;
    gchar   *settingsJson = json_gobject_to_data(G_OBJECT(currentSettings), &settingsJsonLen);
    g_assert_true(NULL != settingsJson);
    g_print("Settings:\n%s\n", settingsJson);
    g_free(settingsJson);
}

//---------------------------------------------------------------------------------------------------------------------