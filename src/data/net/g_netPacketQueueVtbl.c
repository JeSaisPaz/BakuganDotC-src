// bdc 0x08af59e4 g_netPacketQueueVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_netPacketQueueVtbl = { {0}, { .fn = (void *)NetPacketQueueDtor }, { .fn = (void *)NetPacketQueueDebugDump } };
