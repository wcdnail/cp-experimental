#pragma once

#include "mesh.h"

PGMesh stlLoadFile(const gchar *pathname);
PGMesh stlLoadResource(const gchar *pathname);
PGMesh stlLoadAsc(const gchar *pathname, GInputStream *istm);
PGMesh stlLoadBin(const gchar *pathname, GInputStream *istm);
