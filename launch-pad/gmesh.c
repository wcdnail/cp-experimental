#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include "gmesh.h"
#include "glog-box.h"

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
    tempMesh->vertex = g_array_new(FALSE, TRUE, sizeof(GLfloat));
    tempMesh->index = g_array_new(FALSE, TRUE, sizeof(GLuint));
    if (!tempMesh->name) {
        errorTitle = "name allocation";
        goto onError;
    }
    if (!tempMesh->vertex) {
        errorTitle = "vertices allocation";
        goto onError;
    }
    if (!tempMesh->vertex) {
        errorTitle = "indices allocation";
        goto onError;
    }
    goto noError;
onError:
    logBoxTrace(LOGBOX_ERROR, "GMESH %s ERROR: [%d] %s\n", errorTitle, errno, strerror(errno));
    meshFree(tempMesh);
    tempMesh = NULL;
noError:
    return tempMesh;
}

void meshFree(PGMesh mesh)
{
    if (mesh) {
        if (mesh->index) {
            g_array_free(mesh->index, TRUE);
            mesh->index = NULL;
        }
        if (mesh->vertex) {
            g_array_free(mesh->vertex, TRUE);
            mesh->vertex = NULL;
        }
        if (mesh->name) {
            g_string_free(mesh->name, TRUE);
            mesh->name = NULL;
        }
        g_free(mesh);
    }
}

void meshRender(PGMesh mesh)
{
    if(!mesh || !mesh->vertex || !mesh->index) {
        return ;
    }
    glPushMatrix();
    glColor3f(0.5, 0.5, 1.0);

    glPopMatrix();    
}
