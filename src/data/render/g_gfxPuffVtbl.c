// bdc 0x08af1704 g_gfxPuffVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_gfxPuffVtbl = { {0}, { .fn = (void *)GfxPuffDtor }, { .fn = (void *)GfxPuffUpdate } };
