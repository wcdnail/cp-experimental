#pragma once

struct _GVertex
{
    gdouble x, y, z;
};

typedef struct _GVertex   GVertex;
typedef struct _GVertex* PGVertex;

struct _GTriangle 
{
     GVertex vertex[3]; 
     GVertex    normal; 
};

typedef struct _GTriangle   GTriangle;
typedef struct _GTriangle* PGTriangle;

struct _GMesh
{
    GString        *name;
    GString *description;
    guint32        color;
    GArray    *triangles;
};

typedef struct _GMesh   GMesh;
typedef struct _GMesh* PGMesh;

gboolean vertexFromString(PGVertex vertex, const gchar *str, int xindex, int yindex, int zindex);
PGMesh meshNew(const gchar *name);
void meshFree(PGMesh mesh);
void meshRender(PGMesh mesh);

#define vertexSet(vert, NX, NY, NZ) \
    do {                            \
        vert.x = (NX);              \
        vert.y = (NY);              \
        vert.z = (NZ);              \
    } while (0)
