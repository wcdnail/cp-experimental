#pragma once

#include "mainwin.h"
#include "misc.h"

enum _AppSettingsFields {
    PROP_APPSETTING_0,
    PROP_APPSETTING_ID,
    PROP_APPSETTING_TITLE,
    PROP_APPSETTING_RECT,
    PROP_APPSETTING_ISMAX,
    PROP_APPSETTING_LOG_CY,
    PROP_APPSETTING_CONF_CX,
};

struct _AppSettings {
    GObject           super;
    guint                id;
    gchar         *appTitle;
    GRect          *appRect;
    gboolean appIsMaximized;
    gint         logPanelCy;
    gint        confPanelCx;
};

typedef struct _AppSettings   AppSettings;
typedef struct _AppSettings *PAppSettings;

PAppSettings appSettings(void);
void appSettingsFree(void);
void appSettingsSave(void);
void appSettingsOnWindowInit(GMainWin *win);
void appSettingsOnWindowClose(GMainWin *win);
