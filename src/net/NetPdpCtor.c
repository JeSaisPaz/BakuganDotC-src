// bdc 0x089d2e68 NetPdpCtor
#include "bdc.h"

/* Constructor of the 0x14-byte `CONetPDP` object: state 0, both PDP ids -1, a 0x2000-byte packet
   buffer (low heap) at `+0xc`, timeout bytes cleared. Returns `pdp`. */

NetPdp *NetPdpCtor(NetPdp *self)

{
  bool fromLow;
  u8 *buf;
  
  self->state = 0;
  self->recvId = -1;
  self->sendId = -1;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  buf = MemAlloc(0x2000 * sizeof(u8),(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->buffer = buf;
  self->timedOut = '\0';
  self->packetReceived = '\0';
  return self;
}

