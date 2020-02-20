#pragma once

#if defined(_MSC_VER) && (_MAKE_LIBS_SHARED)
#  if defined(_DEFINE_DLL_EXPORT)
#    define LIB_PUBLIC __declspec(dllexport)
#  else
#    define LIB_PUBLIC __declspec(dllimport)
#endif
#endif

#ifndef LIB_PUBLIC
#define LIB_PUBLIC
#endif
