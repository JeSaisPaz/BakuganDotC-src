// bdc 0x08af46ec g_btlDemoScbEventTableVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_btlDemoScbEventTableVtbl = { {0}, { .fn = (void *)BtlDemoScbEventTableDtor } };
