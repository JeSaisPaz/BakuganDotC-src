// bdc 0x08af4714 g_btlDemoScbCueEventVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_btlDemoScbCueEventVtbl = { {0}, { .fn = (void *)BtlDemoScbCueEventDtor }, { .fn = (void *)BtlDemoScbCueEventParse } };
