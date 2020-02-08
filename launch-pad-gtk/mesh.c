#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include "mesh.h"
#include "logbox.h"
#include <errno.h>

gboolean vertexFromString(PGVertex vertex, gchar *str, gsize len)
{
    gchar          *vstr;
    gdouble  *vertexData;
    if (!vertex || !str) {
        return (FALSE);
    }
    vertexData = &vertex->x;
    vstr = str;
    for (gint i = 0; i < 3; i++) {
        if (!(*vstr)) {
            return (FALSE);
        }
        vertexData[i] = g_ascii_strtod(vstr, &vstr);
    }
    return (TRUE);
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
    tempMesh->color = 0xffaf7f7f;
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

void triangleDump(PGTriangle triangle)
{
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
}

static inline void meshPutVertexTriangle(PGTriangle triangle)
{
    glNormal3d(triangle->normal.x, triangle->normal.y, triangle->normal.z);
    for (guint i = 0; i < 3; i++) {
        glVertex3d(triangle->vertex[i].x, triangle->vertex[i].y, triangle->vertex[i].z);
    }
}

static void meshPutTriangles(PGMesh mesh)
{
    glColor4ubv((const GLubyte*)&mesh->color);
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
    glRotated(-90, 1, 0, 0);
    meshPutTriangles(mesh);
    glPopMatrix();
}
