#pragma once

#include <stdarg.h>

enum _LogBoxMessageFlags 
{
    LOGBOX_MSG,
    LOGBOX_NOTE,
    LOGBOX_ASSERT,
    LOGBOX_WARN,
    LOGBOX_ERROR,
};

void logBoxInit(GtkTextView *view);
void logBoxTrace(gint flags, const gchar *format, ...);
void logBox_OnToggleScrollDown(GtkToggleToolButton *btn, gpointer user);
void logBox_OnClear(GtkToolButton *btn, gpointer user);

#ifdef _DEBUG
#define DGB_FUNC_ENTER(Domain) \
    logBoxTrace(LOGBOX_NOTE, #Domain " [%s]\n", __PRETTY_FUNCTION__)
#else
#define DGB_FUNC_ENTER(Domain) \
    do {} while (0)
#endif
