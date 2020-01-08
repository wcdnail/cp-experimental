#pragma once

void gmodelView_Init(GtkGLArea *ctl);
void gmodelView_Free(GtkGLArea *ctl);
gboolean gmodelView_OnRender(GtkGLArea *ctl, gpointer user);
