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

void gmodelView_Init(GMainWin *win);
void gmodelView_Free(GMainWin *win);
gboolean gmodelView_OnRender(GMainWin *win);
