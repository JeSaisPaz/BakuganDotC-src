// bdc 0x08af583c g_gfxSpriteVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_gfxSpriteVtbl = { {0}, { .fn = (void *)GfxSpriteDtor }, { .fn = (void *)GfxSpriteUpdate } };
