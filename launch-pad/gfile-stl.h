#pragma once

#include "gmesh.h"

PGMesh gfileLoadSTL(const gchar *pathname);
PGMesh gresourseLoadSTLA(const gchar *pathname);
PGMesh gloadSTLA(const gchar *pathname, GInputStream *istm);
PGMesh gloadSTLB(const gchar *pathname, GInputStream *istm);
