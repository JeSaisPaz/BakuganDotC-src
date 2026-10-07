// bdc 0x08af6fb8 g_btlDemoScbObjectVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_btlDemoScbObjectVtbl = { {0}, { .fn = (void *)BtlDemoScbObjectDtor } };
