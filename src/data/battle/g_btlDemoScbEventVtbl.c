// bdc 0x08af46fc g_btlDemoScbEventVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_btlDemoScbEventVtbl = { {0}, { .fn = (void *)BtlDemoScbEventDtor }, { .fn = (void *)BtlDemoScbEventParseBody } };
