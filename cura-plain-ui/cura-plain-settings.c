#include <gtk/gtk.h>
#include "cura-plain-settings.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

#define _DEF_WND_TITLE  "Cura-Plain UI"
#define _DEF_WND_WIDTH  1024
#define _DEF_WND_HEIGHT 768

static CP_APP_SETTINGS defaultAppSettings;

static void rcPutToCenterOfScreen(GdkRectangle *rect)
{
    GdkDisplay   *defDisp = gdk_display_get_default();
    GdkMonitor    *defMon = gdk_display_get_primary_monitor(defDisp);
    GdkRectangle scrnArea = { 0 };
    gdk_monitor_get_workarea(defMon, &scrnArea);
    rect->x = (scrnArea.width - rect->width)/2;
    rect->y = (scrnArea.height - rect->height)/2;
}

PCP_APP_SETTINGS initAppSettings(void)
{
    int wndCx = _DEF_WND_WIDTH;
    int wndCy = _DEF_WND_HEIGHT;
    memset(&defaultAppSettings, 0, sizeof(defaultAppSettings));

    defaultAppSettings.mainAppTitle = _DEF_WND_TITLE;
    defaultAppSettings.rcMainApp.width = wndCx;
    defaultAppSettings.rcMainApp.height = wndCy;
    rcPutToCenterOfScreen(&defaultAppSettings.rcMainApp);
    
    return appSettings();
}

PCP_APP_SETTINGS appSettings(void)
{
    return &defaultAppSettings;
}
