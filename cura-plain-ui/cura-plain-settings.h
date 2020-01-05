#pragma once

#include "grect.h"

enum _AppSettingsFields {
    CP_PROP_APP_TITLE = 1,
    CP_PROP_APP_RECT,
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
