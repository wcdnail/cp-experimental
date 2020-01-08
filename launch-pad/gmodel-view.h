#pragma once

#include "gmesh.h"
#include "gmain-win.h"

struct _GScene
{
    GList *meshes;
};

typedef struct _GScene   GScene;
typedef struct _GScene* PGScene;

PGScene gsceneNew(GList *meshes);
void gsceneFree(PGScene scene);

void gmodelView_Init(GtkGLArea *ctl);
void gmodelView_Free(GtkGLArea *ctl);
void gmodelView_Resize(GtkGLArea *ctl, gint cx, gint cy);
gboolean gmodelView_OnRender(GtkGLArea *ctl, GdkGLContext *context);

