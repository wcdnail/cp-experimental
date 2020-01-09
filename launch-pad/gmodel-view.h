#pragma once

#include "gmesh.h"
#include "gmain-win.h"

struct _GScene
{
    GList *meshes;
};

typedef struct _GScene   GScene;
typedef struct _GScene* PGScene;

PGScene sceneNew(GList *meshes);
void sceneFree(PGScene scene);

void modelView_Init(GtkGLArea *ctl);
void modelView_Free(GtkGLArea *ctl);
void modelView_Resize(GtkGLArea *ctl, gint cx, gint cy);
gboolean modelView_OnEvent(GtkGLArea *ctl, GdkEvent *event, gpointer user);
gboolean modelView_OnRender(GtkGLArea *ctl, GdkGLContext *context);

