// bdc 0x08af45e4 g_btlDemoCamVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_btlDemoCamVtbl = { {0}, { .fn = (void *)BtlDemoCamDtor }, { .fn = (void *)BtlDemoCamUpdate } };
