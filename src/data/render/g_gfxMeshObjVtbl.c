// bdc 0x08af16f4 g_gfxMeshObjVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_gfxMeshObjVtbl = { {0}, { .fn = (void *)GfxMeshObjDtor } };
