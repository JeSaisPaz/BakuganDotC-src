// bdc 0x089ffd58 NetPacketQueueDtor
#include "bdc.h"

/* Destructor of the ad-hoc packet queue (vtable `g_netPacketQueueVtbl` slot 1): runs `CoreBufQueueDtor` and
   frees the object when `flags & 1`. */

void NetPacketQueueDtor(void *queue_, u32 flags)
{
  CoreBufQueue *queue = (CoreBufQueue *)queue_;
  if (queue != (CoreBufQueue *)0x0) {
    queue->vtbl = g_netPacketQueueVtbl;
    CoreBufQueueDtor(queue, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(queue, (const char *)0x0, 0);
      MemUnlock();
    }
  }
}
