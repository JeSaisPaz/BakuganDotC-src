// bdc 0x089ffd1c NetPacketQueueCtor
#include "bdc.h"

/* Constructor of the ad-hoc packet queue: a `CoreBufQueue` of `count` 4-byte
   entries whose vtable is then replaced by `g_netPacketQueueVtbl`. Returns `queue`. Used by `NetAdhocCreate`. */

CoreBufQueue *NetPacketQueueCtor(CoreBufQueue *queue, int count)
{
  CoreBufQueueCtor(queue, 4, count);
  queue->vtbl = g_netPacketQueueVtbl;
  return queue;
}
