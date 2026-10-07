// bdc 0x08af43c4 g_gameFieldPlacementVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_gameFieldPlacementVtbl = { {0}, { .fn = (void *)GameFieldPlacementDtor } };
