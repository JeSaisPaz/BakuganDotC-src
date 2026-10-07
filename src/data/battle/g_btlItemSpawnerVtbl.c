// bdc 0x08af2854 g_btlItemSpawnerVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_btlItemSpawnerVtbl = { {0}, { .fn = (void *)BtlItemSpawnerDtor } };
