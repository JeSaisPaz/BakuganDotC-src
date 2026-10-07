// bdc 0x08af2c84 g_btlItemVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_btlItemVtbl = { {0}, { .fn = (void *)BtlItemDtor } };
