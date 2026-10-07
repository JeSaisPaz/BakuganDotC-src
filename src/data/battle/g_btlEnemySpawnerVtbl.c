// bdc 0x08af2664 g_btlEnemySpawnerVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_btlEnemySpawnerVtbl = { {0}, { .fn = (void *)BtlEnemySpawnerDtor } };
