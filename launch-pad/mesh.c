#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include "mesh.h"
#include "logbox.h"

gboolean vertexFromString(PGVertex vertex, const gchar *str)
{
    gchar **varg = g_strsplit(str, " ", 3);
    if (!varg) {
        return (FALSE);
    }
    vertex->x = g_ascii_strtod(varg[0], NULL);
    vertex->y = g_ascii_strtod(varg[1], NULL);
    vertex->z = g_ascii_strtod(varg[2], NULL);
    g_strfreev(varg);
    return (TRUE);
#ifdef _DEBUG_MESH_DATA
    lgTrace(LG_MSG, "TRI:\n"
        "    A: [%f %f %f]\n"
        "    B: [%f %f %f]\n"
        "    C: [%f %f %f]\n"
        "    N: [%f %f %f]\n",
        triangle->vertex[0].x, triangle->vertex[0].y, triangle->vertex[0].z,
        triangle->vertex[1].x, triangle->vertex[1].y, triangle->vertex[1].z,
        triangle->vertex[2].x, triangle->vertex[2].y, triangle->vertex[2].z,
        triangle->normal.x, triangle->normal.y, triangle->normal.z
    );
#endif
}

PGMesh meshNew(const gchar *name)
{
    const gchar *errorTitle = NULL;
    PGMesh         tempMesh = NULL;
    tempMesh = g_malloc(sizeof(GMesh));
    if (!tempMesh) {
        errorTitle = "mesh allocation";
        goto onError;
    }
    tempMesh->name = g_string_new(name);
    tempMesh->description = NULL;
    tempMesh->color = 0xff000000;
    tempMesh->triangles = NULL;
    if (!tempMesh->name) {
        errorTitle = "name allocation";
        goto onError;
    }
    goto noError;
onError:
    lgTrace(LG_ERROR, "GMESH %s ERROR: [%d] %s\n", errorTitle, errno, strerror(errno));
    meshFree(tempMesh);
    tempMesh = NULL;
noError:
    return tempMesh;
}

void meshFree(PGMesh mesh)
{
    if (mesh) {
        if (mesh->triangles) {
            g_array_free(mesh->triangles, TRUE);
            mesh->triangles = NULL;
        }
        if (mesh->description) {
            g_string_free(mesh->description, TRUE);
            mesh->description = NULL;
        }
        if (mesh->name) {
            g_string_free(mesh->name, TRUE);
            mesh->name = NULL;
        }
        g_free(mesh);
    }
}

static void meshPutVertexTriangle(PGTriangle triangle)
{
    //glNormal3d(triangle->normal.x, triangle->normal.y, triangle->normal.z);
    for (guint i = 0; i < 3; i++) {
        glVertex3d(triangle->vertex[i].x, triangle->vertex[i].y, triangle->vertex[i].z);
    }
}

static void meshPutTriangles(PGMesh mesh)
{
    //guint32 wirecolor = 0xff000000;
    //glColor4bv((const GLbyte*)&wirecolor);
    glColor4bv((const GLbyte*)&mesh->color);
    glBegin(GL_TRIANGLES);
    for (guint i = 0; i < mesh->triangles->len; i++) {
        PGTriangle trianle = &g_array_index(mesh->triangles, GTriangle, i);
        meshPutVertexTriangle(trianle);
    }
    glEnd();
}

void meshRender(PGMesh mesh)
{
    guint tricount;
    if(!mesh || !mesh->triangles) {
        return ;
    }
    glPushMatrix();
    meshPutTriangles(mesh);
    //meshPutLines(mesh);
    glPopMatrix();
}
