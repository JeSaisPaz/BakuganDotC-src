// bdc 0x089d2dc4 NetPdpDestroy
#include "bdc.h"

/* Destroys the `CONetPDP` object (`NetPdpDtor` with flags 3) and frees the holder `*0x08ac5978`.
   Final state of `NetPdpStep`. */

void NetPdpDestroy(void)

{
  NetPdpState *holder;
  
  holder = g_netPdpState;
  if (g_netPdpState->pdp != (NetPdp *)0x0) {
    NetPdpDtor(g_netPdpState->pdp,3);
    holder = g_netPdpState;
    g_netPdpState->pdp = (NetPdp *)0x0;
  }
  if (holder != (NetPdpState *)0x0) {
    MemLock();
    MemFree(g_netPdpState,(char *)0x0,0);
    MemUnlock();
    g_netPdpState = (NetPdpState *)0x0;
  }
  return;
}

