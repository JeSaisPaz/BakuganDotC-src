// bdc 0x08af16cc g_gfxEffectVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_gfxEffectVtbl = { {0}, { .fn = (void *)GfxEffectDtor }, { .fn = (void *)GfxEffectUpdate } };
