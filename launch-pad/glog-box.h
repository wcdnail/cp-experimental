#pragma once

#include <stdarg.h>

enum _LogBoxMessageFlags 
{
    LOGBOX_MSG,
    LOGBOX_NOTE,
    LOGBOX_WARN,
    LOGBOX_ERROR,
};

void logBoxInit(GtkTextView *view);
void logBoxTrace(gint flags, const gchar *format, ...);
