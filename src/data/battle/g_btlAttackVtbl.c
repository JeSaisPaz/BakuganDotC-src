// bdc 0x08af2164 g_btlAttackVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_btlAttackVtbl = { {0}, { .fn = (void *)BtlAttackDtor } };
