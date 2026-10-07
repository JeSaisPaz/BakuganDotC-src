// bdc 0x08af571c g_gfxRectBaseVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_gfxRectBaseVtbl = { {0}, { .fn = (void *)GfxRectBaseDtor }, { .fn = (void *)GfxRectBaseDlWriteQuad } };
