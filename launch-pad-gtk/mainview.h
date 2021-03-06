#pragma once

#include "mesh.h"
#include "mainwin.h"

struct _GScene
{
    gboolean isPerspectiveView;
    GVertex           position;
    GVertex           rotation;
    gboolean        isRotating;
    double                  lx;
    double                  ly;
    GList              *meshes;
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

#define sceneSetPosition(SCENE, NX, NY, NZ) vertexSet(SCENE->position, NX, NY, NZ)
#define sceneSetRotation(SCENE, NX, NY, NZ) vertexSet(SCENE->rotation, NX, NY, NZ)
