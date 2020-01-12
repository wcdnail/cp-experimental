#pragma once

struct _GVertex
{
    double x;
    double y;
    double z;
};

typedef struct _GVertex   GVertex;
typedef struct _GVertex* PGVertex;

struct _GMesh
{
    GString  *name;
    GArray *vertex;
    GArray  *index;
};

typedef struct _GMesh   GMesh;
typedef struct _GMesh* PGMesh;

PGMesh meshNew(const gchar *name);
void meshFree(PGMesh mesh);
void meshRender(PGMesh mesh);

#define vertexSet(vert, NX, NY, NZ) \
    do {                            \
        vert.x = (NX);              \
        vert.y = (NY);              \
        vert.z = (NZ);              \
    } while (0)
