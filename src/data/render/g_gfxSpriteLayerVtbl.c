// bdc 0x08af5854 g_gfxSpriteLayerVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_gfxSpriteLayerVtbl = { {0}, { .fn = (void *)GfxSpriteLayerDtor } };
