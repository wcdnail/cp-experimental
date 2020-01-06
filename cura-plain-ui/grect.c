#include <gtk/gtk.h>
#include "grect.h"
#include <glib-object.h>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

#define _DEF_GRECT_MAX_VALUE 1000000

struct _GRectClass
{
    GObjectClass parent;
};

typedef struct _GRectClass GRectClass;

#define GRECT_TYPE_OBJECT                (grect_get_type())
#define GRECT_OBJECT(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), GRECT_TYPE_OBJECT, GRect))
/*
#define GRECT_IS_OBJECT(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), GRECT_TYPE_OBJECT))
#define GRECT_OBJECT_CLASS(cls)          (G_TYPE_CHECK_CLASS_CAST((cls), GRECT_TYPE_OBJECT, GRectClass))
#define GRECT_IS_OBJECT_CLASS(cls)       (G_TYPE_CHECK_CLASS_TYPE((cls), GRECT_TYPE_OBJECT))
#define GRECT_OBJECT_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), GRECT_TYPE_OBJECT, GRectClass))
*/

G_DEFINE_TYPE(GRect, grect, G_TYPE_OBJECT);

static void grect_set_property(GObject *gobject, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    switch (prop_id)
    {
    case GRC_PROP_X:  GRECT_OBJECT(gobject)->x = g_value_get_int(value); break;
    case GRC_PROP_Y:  GRECT_OBJECT(gobject)->y = g_value_get_int(value); break;
    case GRC_PROP_CX: GRECT_OBJECT(gobject)->cx = g_value_get_int(value); break;
    case GRC_PROP_CY: GRECT_OBJECT(gobject)->cy = g_value_get_int(value); break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);
    }
}

static void grect_get_property(GObject *gobject, guint prop_id, GValue *value, GParamSpec *pspec)
{
    switch (prop_id)
    {
    case GRC_PROP_X:  g_value_set_int(value, GRECT_OBJECT(gobject)->x); break;
    case GRC_PROP_Y:  g_value_set_int(value, GRECT_OBJECT(gobject)->y); break;
    case GRC_PROP_CX: g_value_set_int(value, GRECT_OBJECT(gobject)->cx); break;
    case GRC_PROP_CY: g_value_set_int(value, GRECT_OBJECT(gobject)->cy); break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);
    }
}

static void grect_class_init(GRectClass *cls)
{
    GObjectClass *gobject = G_OBJECT_CLASS(cls);
    gobject->set_property = grect_set_property;
    gobject->get_property = grect_get_property;
    g_object_class_install_property(gobject, GRC_PROP_X, g_param_spec_int("x", "X", "", 0, _DEF_GRECT_MAX_VALUE, 0, G_PARAM_READWRITE));
    g_object_class_install_property(gobject, GRC_PROP_Y, g_param_spec_int("y", "Y", "", 0, _DEF_GRECT_MAX_VALUE, 0, G_PARAM_READWRITE));
    g_object_class_install_property(gobject, GRC_PROP_CX, g_param_spec_int("cx", "CX", "", 0, _DEF_GRECT_MAX_VALUE, 0, G_PARAM_READWRITE));
    g_object_class_install_property(gobject, GRC_PROP_CY, g_param_spec_int("cy", "CY", "", 0, _DEF_GRECT_MAX_VALUE, 0, G_PARAM_READWRITE));
}

static void grect_init(GRect *obj)
{
    obj->x = 0;
    obj->y = 0;
    obj->cx = 0;
    obj->cy = 0;
}

//---------------------------------------------------------------------------------------------------------------------

PGRect grect_new(void)
{
    return g_object_new(GRECT_TYPE_OBJECT, NULL);
}

PGRect grect_new_size(gint cx, gint cy)
{
    PGRect rv = grect_new();
    g_assert_true(NULL != rv);
    rv->cx = cx;
    rv->cy = cy;
    return rv;
}

void grect_set_rect(PGRect targ, PGRect src)
{
    g_assert_true(NULL != targ);
    g_assert_true(NULL != src);
    targ->x = src->x;
    targ->y = src->y;
    targ->cx = src->cx;
    targ->cy = src->cy;
}

void grect_put_to_center_of_screen(PGRect rect)
{
    GdkDisplay   *defDisp = gdk_display_get_default();
    GdkMonitor    *defMon = gdk_display_get_primary_monitor(defDisp);
    GdkRectangle scrnArea = { 0 };
    gdk_monitor_get_workarea(defMon, &scrnArea);
    rect->x = (scrnArea.width - rect->cx)/2;
    rect->y = (scrnArea.height - rect->cy)/2;
}
