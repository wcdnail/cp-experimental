#pragma once

struct _GVertex
{
    float pos[3];
};

typedef struct _GVertex   GVertex;
typedef struct _GVertex* PGVertex;

struct _GMesh
{
    GString  *name;
    GArray *vertex; // GVertex
    GArray  *index; // GLuint [2][]
};

typedef struct _GMesh   GMesh;
typedef struct _GMesh* PGMesh;

PGMesh gmeshNew(const gchar *name);
void gmeshFree(PGMesh mesh);
void gmeshRender(PGMesh mesh);
