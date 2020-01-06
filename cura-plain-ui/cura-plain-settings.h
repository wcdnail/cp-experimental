#pragma once

#include "grect.h"

enum _AppSettingsFields {
    PROP_APPSETTING_0,
    PROP_APPSETTING_TITLE,
    PROP_APPSETTING_RECT,
};

struct _AppSettings {
    GObject  parent;
    gchar *appTitle;
    GRect  *appRect;
};

typedef struct _AppSettings   AppSettings;
typedef struct _AppSettings *PAppSettings;

PAppSettings appSettings(void);
void appSettingsFree(void);
void appSettingSave(void);
