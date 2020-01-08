#pragma once

struct _GMesh
{
    GString  *name;
    GArray *vertex; // GLfloat [3][]
    GArray  *index; // GLuint [2][]
};

typedef struct _GMesh   GMesh;
typedef struct _GMesh* PGMesh;

PGMesh gmeshNew(const gchar *name);
void gmeshFree(PGMesh mesh);
void gmeshRender(PGMesh mesh);
