// bdc 0x08af2b84 g_stopWallVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_stopWallVtbl = { {0}, { .fn = (void *)StopWallDtor } };
