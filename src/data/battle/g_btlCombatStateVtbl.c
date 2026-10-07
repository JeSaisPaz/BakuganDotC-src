// bdc 0x08af2184 g_btlCombatStateVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_btlCombatStateVtbl = { {0}, { .fn = (void *)BtlCombatDtor } };
