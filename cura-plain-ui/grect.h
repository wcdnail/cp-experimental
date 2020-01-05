#pragma once

enum _G_RECT_FLDS {
    G_RC_X = 1,
    G_RC_Y,
    G_RC_CX,
    G_RC_CY,
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
