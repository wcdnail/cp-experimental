#include <gtk/gtk.h>
#include "glog-box.h"

static      GtkTextView *mainLogBox = NULL;
static GtkTextBuffer *mainLogBuffer = NULL;
static gboolean   mainLogScrollDown = TRUE;

static void glog_trace_impl(gint flags, const gchar *format, va_list ap);

void logBoxInit(GtkTextView *view) 
{
    mainLogBox = view;
    if(mainLogBox) {
        mainLogBuffer = gtk_text_view_get_buffer(mainLogBox);
        gtk_text_buffer_create_tag(mainLogBuffer, "msghead_fg", "foreground", "LIGHTCYAN", NULL); 
        gtk_text_buffer_create_tag(mainLogBuffer, "msghead_bg", "background", "STEELBLUE", NULL); 
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
    glog_trace_impl(flags, format, ap);
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

static void glog_scroll_to_end_ui(void)
{
    GtkTextIter  iter;
    GtkTextMark* mark;
    gtk_text_buffer_get_end_iter(mainLogBuffer, &iter);
    mark = gtk_text_buffer_create_mark(mainLogBuffer, NULL, &iter, FALSE);
    gtk_text_view_scroll_mark_onscreen(mainLogBox, mark);
    gtk_text_buffer_delete_mark(mainLogBuffer, mark);
}

static void glog_put_text_ui(gint flags, gchar *title, GString *message)
{
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(mainLogBuffer, &iter);
    gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, title, -1, "bold", 
        LOGBOX_ERROR == flags ? "error_bg" : "msghead_bg", 
        LOGBOX_ERROR == flags ? "error_fg" : "msghead_fg", 
        NULL);
    gtk_text_buffer_get_end_iter(mainLogBuffer, &iter);
    switch (flags) {
    case LOGBOX_ASSERT: gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, message->str, (gint)message->len, "bold", NULL); break;
    case LOGBOX_NOTE:   gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, message->str, (gint)message->len, "italic", NULL); break;
    case LOGBOX_WARN:   gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, message->str, (gint)message->len, "bold", "warn_bg", "warn_fg", NULL); break;
    case LOGBOX_ERROR:  gtk_text_buffer_insert_with_tags_by_name(mainLogBuffer, &iter, message->str, (gint)message->len, "bold", "error_bg", "error_fg", NULL); break;
    default:            gtk_text_buffer_insert(mainLogBuffer, &iter, message->str, (gint)message->len);
    }
    if (mainLogScrollDown) {
        glog_scroll_to_end_ui();
    }
}

#define _MSG_TIMEBUF_MAX_LEN 64
#define _MSG_HEAD_MAX_LEN   128

static void glog_make_msg_title(gchar *title) 
{
    gint64       now = g_get_real_time();
    time_t   nowsecs = (time_t)(now / 1000000);
    struct tm *nowtm = localtime(&nowsecs);
    gchar tmbuf[_MSG_TIMEBUF_MAX_LEN] = { 0 };
    strftime(tmbuf, _MSG_TIMEBUF_MAX_LEN, "%H:%M:%S", nowtm);
    g_snprintf(title, _MSG_HEAD_MAX_LEN, "  %s.%06d:  ", tmbuf, (gint)((now / 1000000) % 1000000));
}

static void glog_trace_impl(gint flags, const gchar *format, va_list ap)
{
    gchar title[_MSG_HEAD_MAX_LEN] = { 0 };
    GString *message = g_string_new("  ");
    if (!message) {
        return ;
    }
    g_string_append_vprintf(message, format, ap);
    glog_make_msg_title(title);
    g_print("%s%s", title, message->str);
    if (mainLogBox) {
        glog_put_text_ui(flags, title, message);
    }
    g_string_free(message, TRUE);
}
