// bdc 0x08af1afc g_btlLoadRequestVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_btlLoadRequestVtbl = { {0}, { .fn = (void *)BtlLoadRequestDtor } };
