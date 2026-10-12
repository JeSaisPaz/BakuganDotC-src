// bdc 0x08af532c g_padBaseVtable
#include "bdc.h"

__typeof__(VtblEntry[2]) g_padBaseVtable = { {0}, { .fn = (void *)PadBaseDtor } };
