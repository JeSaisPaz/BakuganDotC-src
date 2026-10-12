// bdc 0x08af5884 g_gfxFabClipVtable
#include "bdc.h"

__typeof__(VtblEntry[2]) g_gfxFabClipVtable = { {0}, { .fn = (void *)GfxFabClipDtor } };
