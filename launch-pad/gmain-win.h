#pragma once

#include "glaunch-pad.h"

#define GMAIN_WIN_TYPE (gmain_win_get_type())

G_DECLARE_FINAL_TYPE(GMainWin, gmain_win, GMAIN, WIN, GtkApplicationWindow)

struct _GMainWin
{
    GtkApplicationWindow                  super;
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

GMainWin *mainWinNew(GLaunchPad *app);
void mainWinOpen(GMainWin *win, GFile *file);
