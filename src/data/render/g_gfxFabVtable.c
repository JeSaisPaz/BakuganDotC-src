// bdc 0x08af5874 g_gfxFabVtable
#include "bdc.h"

__typeof__(VtblEntry[2]) g_gfxFabVtable = { {0}, { .fn = (void *)GfxFabDtor } };
