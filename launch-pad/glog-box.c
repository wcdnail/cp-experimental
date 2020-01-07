#include <gtk/gtk.h>
#include "glog-box.h"

static      GtkTextView *mainLogBox = NULL;
static GtkTextBuffer *mainLogBuffer = NULL;
static gboolean   mainLogScrollDown = TRUE;

static void logBoxTraceImpl(gint flags, const gchar *format, va_list ap);

void logBoxInit(GtkTextView *view) 
{
    mainLogBox = view;
    if(mainLogBox) {
        mainLogBuffer = gtk_text_view_get_buffer(mainLogBox);
        gtk_text_buffer_create_tag(mainLogBuffer, "time_fg", "foreground", "LIGHTCYAN", NULL); 
        gtk_text_buffer_create_tag(mainLogBuffer, "time_bg", "background", "STEELBLUE", NULL); 
        gtk_text_buffer_create_tag(mainLogBuffer, "warn_fg", "foreground", "BLUE", NULL); 
        gtk_text_buffer_create_tag(mainLogBuffer, "warn_bg", "background", "WHITE", NULL); 
        gtk_text_buffer_create_tag(mainLogBuffer, "error_fg", "foreground", "YELLOW", NULL); 
        gtk_text_buffer_create_tag(mainLogBuffer, "error_bg", "background", "ORANGERED", NULL); 
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

void logBox_OnToggleScrollDown(GtkToggleToolButton *btn, gpointer user)
{
    mainLogScrollDown = gtk_toggle_tool_button_get_active(btn);
}

void logBox_OnClear(GtkToolButton *btn, gpointer user)
{
    GtkTextIter beg;
    GtkTextIter end;
    gtk_text_buffer_get_bounds(mainLogBuffer, &beg, &end);
    gtk_text_buffer_delete(mainLogBuffer, &beg, &end);
}

static void scrollToEnd(void)
{
    GtkTextIter  iter;
    GtkTextMark* mark;
    gtk_text_buffer_get_end_iter(mainLogBuffer, &iter);
    mark = gtk_text_buffer_create_mark(mainLogBuffer, NULL, &iter, FALSE);
    gtk_text_view_scroll_mark_onscreen(mainLogBox, mark);
    gtk_text_buffer_delete_mark(mainLogBuffer, mark);
}

static void putTextUI(gint flags, GString *message)
{
    GtkTextIter iter;
    gchar *timeStamp = g_date_time_format(g_date_time_new_now_local(), " %T ");
    gtk_text_buffer_get_end_iter(mainLogBuffer, &iter);
    if (timeStamp) {
        gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, timeStamp, -1, "bold", 
            LOGBOX_ERROR == flags ? "error_bg" : "time_bg", 
            LOGBOX_ERROR == flags ? "error_fg" : "time_fg", 
            NULL);
        gtk_text_buffer_get_end_iter(mainLogBuffer, &iter);
        g_free(timeStamp);
    }
    switch (flags) {
    case LOGBOX_NOTE:  gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, message->str, (gint)message->len, "italic", NULL); break;
    case LOGBOX_WARN:  gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, message->str, (gint)message->len, "bold", "warn_bg", "warn_fg", NULL); break;
    case LOGBOX_ERROR: gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, message->str, (gint)message->len, "bold", "error_bg", "error_fg", NULL); break;
    default:           gtk_text_buffer_insert(mainLogBuffer, &iter, message->str, (gint)message->len);
    }
    if (mainLogScrollDown) {
        scrollToEnd();
    }
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
