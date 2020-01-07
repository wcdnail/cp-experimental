#pragma once

#include "glaunch-pad.h"

#define GMAIN_WIN_TYPE (gmain_win_get_type())

G_DECLARE_FINAL_TYPE(GMainWin, gmain_win, GMAIN, WIN, GtkApplicationWindow)

struct _GMainWin
{
    GtkApplicationWindow                  super;
    GSettings                         *settings;
    GtkPaned                           *panRoot;
    GtkPaned                           *panView;
    GtkGLArea                        *modelView;
    GtkTreeView                     *configTree;
    GtkTreeSelection             *configTreeSel;
    GtkTextView                         *logBox;
    GtkToggleToolButton *logctlToggleScrollDown;
    GtkEntry           *cmdSingleCommandEditBox;
    GtkToolButton          *cmdRunSingleCommand;
};

GMainWin *gmain_win_new(GLaunchPad *app);
void gmain_win_open(GMainWin *win, GFile *file);
