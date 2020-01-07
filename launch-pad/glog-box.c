#include <gtk/gtk.h>
#include "glog-box.h"

static      GtkTextView *mainLogBox = NULL;
static GtkTextBuffer *mainLogBuffer = NULL;

static void logBoxTraceImpl(gint flags, const gchar *format, va_list ap);

void logBoxInit(GtkTextView *view) 
{
    mainLogBox = view;
    if(mainLogBox) {
        mainLogBuffer = gtk_text_view_get_buffer(mainLogBox);
        gtk_text_buffer_create_tag(mainLogBuffer, "time_fg", "foreground", "LIGHTCYAN", NULL); 
        gtk_text_buffer_create_tag(mainLogBuffer, "time_bg", "background", "STEELBLUE", NULL); 
        gtk_text_buffer_create_tag(mainLogBuffer, "warn_fg", "foreground", "PURPLE", NULL); 
        gtk_text_buffer_create_tag(mainLogBuffer, "error_fg", "foreground", "LEMONCHIFFON", NULL); 
        gtk_text_buffer_create_tag(mainLogBuffer, "error_bg", "background", "TOMATO", NULL); 
        gtk_text_buffer_create_tag(mainLogBuffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
        gtk_text_buffer_create_tag(mainLogBuffer, "bold", "weight", PANGO_WEIGHT_BOLD, NULL);
    }
}

void logBoxTrace(gint flags, const gchar *format, ...)
{
    va_list ap;
    va_start(ap, format);
    logBoxTraceImpl(flags, format, ap);
    va_end(ap);
}

static void scrollToEnd(gboolean waitUi) 
{
    GtkTextIter  iter;
    GtkTextMark* mark;
    int     lineCount = 0;
    if (waitUi) {
        // Wait for Gui to Redraw with added line to buffer
        while (gtk_events_pending()) {
            gtk_main_iteration_do(FALSE);
        }
    }
    //lineCount = gtk_text_buffer_get_line_count(mainLogBuffer);
    //gtk_text_buffer_get_iter_at_line(mainLogBuffer, &iter, lineCount);
    gtk_text_buffer_get_end_iter(mainLogBuffer, &iter);
    mark = gtk_text_buffer_create_mark(mainLogBuffer, NULL, &iter, 1);
    gtk_text_view_scroll_to_mark(mainLogBox, mark, 0.0, 0, 0.0, 1.0);
    gtk_text_buffer_delete_mark (mainLogBuffer, mark);
    //gtk_text_view_scroll_to_iter(mainLogBox, &iter, 0, FALSE, 0, 0);
}

static void putTextUI(gint flags, GString *message)
{
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(mainLogBuffer, &iter);
    gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, "  <TIMESTAMP>  ", -1, "bold", "time_bg", "time_fg", NULL);
    gtk_text_buffer_get_end_iter(mainLogBuffer, &iter);
    gtk_text_buffer_insert(mainLogBuffer, &iter, "  ", -1);
    gtk_text_buffer_get_end_iter(mainLogBuffer, &iter);
    switch (flags) {
    case LOGBOX_NOTE:
        gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, message->str, (gint)message->len, "italic", NULL);
        break;
    case LOGBOX_WARN:
        gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, message->str, (gint)message->len, "bold", "warn_fg", NULL);
        break;
    case LOGBOX_ERROR:
        gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, message->str, (gint)message->len, "bold", "error_bg", "error_fg", NULL);
        break;
    default:
        gtk_text_buffer_insert(mainLogBuffer, &iter, message->str, (gint)message->len);
    }
    scrollToEnd(TRUE);
}

static void logBoxTraceImpl(gint flags, const gchar *format, va_list ap)
{
    GString *temp = g_string_new(NULL);
    if (!temp) {
        return ;
    }
    g_string_vprintf(temp, format, ap);
    g_print("%s", temp->str);
    if (mainLogBox) {
        putTextUI(flags, temp);
    }
    g_string_free(temp, TRUE);
}
