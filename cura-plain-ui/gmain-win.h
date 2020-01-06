#pragma once

#include "glaunch-pad.h"

#define GMAIN_WIN_TYPE (gmain_win_get_type())

G_DECLARE_FINAL_TYPE(GMainWin, gmain_win, GMAIN, WIN, GtkApplicationWindow)

GMainWin *gmain_win_new(GLaunchPad *app);
void gmain_win_open(GMainWin *win, GFile *file);
