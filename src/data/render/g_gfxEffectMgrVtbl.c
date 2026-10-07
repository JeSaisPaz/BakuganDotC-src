// bdc 0x08af16e4 g_gfxEffectMgrVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_gfxEffectMgrVtbl = { {0}, { .fn = (void *)GfxEffectMgrDtor } };
