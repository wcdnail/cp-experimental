#pragma once

#ifdef _DEBUG
#define DGB_FUNC_ENTER(Domain) \
    g_print(#Domain " [%s]\n", __PRETTY_FUNCTION__)
#else
#define DGB_FUNC_ENTER(Domain) \
    do {} while (0)
#endif
