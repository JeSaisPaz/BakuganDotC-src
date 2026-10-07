// bdc 0x08af6e00 g_gameFieldPointVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_gameFieldPointVtbl = { {0}, { .fn = (void *)GameFieldPointDtor } };
