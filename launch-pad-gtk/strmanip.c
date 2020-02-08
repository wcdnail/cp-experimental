#include <gtk/gtk.h>
#include "strmanip.h"
#include <ctype.h>

const gchar* strFirstNotSpace(const gchar *start, gsize *len)
{
    const gchar *p = start;
    while (isspace(*p) && (*len) > 1) {
        ++p;
        --(*len);
    }
    return (p);
}

const gchar* strFirstSpace(const gchar *start, gsize *len)
{
    const gchar *p = start;
    while (!isspace(*p) && (*len) > 1) {
        ++p;
        --(*len);
    }
    return (p);
}

