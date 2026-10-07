// bdc 0x089d2ce0 NetPdpCreate
#include "bdc.h"

/* Allocates the 0x28-byte PDP holder `g_netPdpState` (zeroed) and its `CONetPDP` object
   (`NetPdpCtor`) at `+0`; `pdp` stays NULL if the object allocation fails. */
void NetPdpCreate(void)
{
  bool prevLow;
  NetPdpState *state;
  NetPdp *self;
  NetPdp *pdp;

  MemLock();
  prevLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  state = MemAlloc(sizeof(NetPdpState),(char *)0x0,0);
  MemSetAllocFromLow(prevLow);
  MemUnlock();
  g_netPdpState = state;
  memset(state,0,sizeof(NetPdpState));
  MemLock();
  prevLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  self = MemAlloc(sizeof(NetPdp),(char *)0x0,0);
  MemSetAllocFromLow(prevLow);
  MemUnlock();
  pdp = (NetPdp *)0x0;
  if (self != (NetPdp *)0x0) {
    NetPdpCtor(self);
    pdp = self;
  }
  g_netPdpState->pdp = pdp;
  return;
}
