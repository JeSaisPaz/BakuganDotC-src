// bdc 0x08af59cc g_coreBufQueueVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_coreBufQueueVtbl = { {0}, { .fn = (void *)CoreBufQueueDtor }, { .fn = (void *)CoreBufQueueDebugDump } };
