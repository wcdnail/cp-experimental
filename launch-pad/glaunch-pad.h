#pragma once

#define _DEF_APP_ID      "cura-plain"
#define GLAUNCH_PAD_TYPE (glaunch_pad_get_type())

G_DECLARE_FINAL_TYPE(GLaunchPad, glaunch_pad, GLAUNCH, PAD, GtkApplication)

GLaunchPad *glaunch_pad_new(void);
