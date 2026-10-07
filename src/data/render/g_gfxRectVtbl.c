// bdc 0x08af5734 g_gfxRectVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_gfxRectVtbl = { {0}, { .fn = (void *)GfxRectDtor }, { .fn = (void *)GfxRectDlWriteQuad } };
