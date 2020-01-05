#pragma once

struct _CP_APP_SETTINGS {
    const char *mainAppTitle;
    GdkRectangle   rcMainApp;
};

typedef struct _CP_APP_SETTINGS   CP_APP_SETTINGS;
typedef struct _CP_APP_SETTINGS *PCP_APP_SETTINGS;

PCP_APP_SETTINGS appSettings(void);
PCP_APP_SETTINGS initAppSettings(void);
