#include "stdafx.h"
#include "trace-it.h"

namespace trace
{
    void LIB_PUBLIC plainOut(const char *msg)
    {
#ifdef _WIN32
        ::OutputDebugStringA(msg);
#endif
    }
}
