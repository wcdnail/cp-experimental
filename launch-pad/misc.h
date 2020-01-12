#pragma once

enum _GRectFields {
    GRC_PROP_0,
    GRC_PROP_X,
    GRC_PROP_Y,
    GRC_PROP_CX,
    GRC_PROP_CY,
};

struct _GRect {
    GObject parent;
    gint         x;
    gint         y;
    gint        cx;
    gint        cy;
};

typedef struct _GRect   GRect;
typedef struct _GRect *PGRect;

GType grect_get_type(void);
PGRect grect_new(void);
PGRect grect_new_size(gint cx, gint cy);
void grect_put_to_center_of_screen(PGRect rect);
void grect_set_rect(PGRect targ, PGRect src);
