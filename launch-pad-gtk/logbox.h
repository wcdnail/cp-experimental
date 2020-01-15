#pragma once

#include <stdarg.h>

enum _LG_MsgFlags
{
    LG_MSG,
    LG_NOTE,
    LG_ASSERT,
    LG_WARN,
    LG_ERROR,
};

void logBoxInit(GtkTextView *view);
void lgTrace(gint flags, const gchar *format, ...);
void logBox_OnToggleScrollDown(GtkToggleToolButton *btn, gpointer user);
void logBox_OnClear(GtkToolButton *btn, gpointer user);

#ifdef _DEBUG
#define DGB_FUNC_ENTER(Domain) \
    lgTrace(LG_NOTE, #Domain " [%s]\n", __PRETTY_FUNCTION__)
#else
#define DGB_FUNC_ENTER(Domain) \
    do {} while (0)
#endif
