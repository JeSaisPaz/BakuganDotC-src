// bdc 0x08af5864 g_gfxTextureVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_gfxTextureVtbl = { {0}, { .fn = (void *)GfxTextureDtor } };
