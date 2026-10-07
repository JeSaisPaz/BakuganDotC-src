// bdc 0x08af2174 g_btlInputVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_btlInputVtbl = { {0}, { .fn = (void *)BtlInputDtor } };
