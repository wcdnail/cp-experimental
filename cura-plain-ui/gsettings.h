#pragma once

#include "grect.h"

enum _AppSettingsFields {
    PROP_APPSETTING_0,
    PROP_APPSETTING_ID,
    PROP_APPSETTING_TITLE,
    PROP_APPSETTING_RECT,
    PROP_APPSETTING_WORK_AREA_RECT,
};

struct _AppSettings {
    GObject     super;
    guint          id;
    gchar   *appTitle;
    GRect    *appRect;
    GRect   *workRect;
};

typedef struct _AppSettings   AppSettings;
typedef struct _AppSettings *PAppSettings;

PAppSettings appSettings(void);
void appSettingsFree(void);
void appSettingSave(void);
