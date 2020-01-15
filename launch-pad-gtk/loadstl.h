#pragma once

#include "mesh.h"

PGMesh stlLoadFile(const gchar *pathname);
PGMesh stlLoadResource(const gchar *pathname);
