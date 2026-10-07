// bdc 0x089ffdcc NetPacketQueueDebugDump
#include "bdc.h"

/* Empty override (`jr ra`) of the CoreBufQueueDebugDump slot (vtable `0x08af59e4` slot 2) for
   the ad-hoc packet queue. */
void NetPacketQueueDebugDump(void *queue)
{
    (void)queue;
}
