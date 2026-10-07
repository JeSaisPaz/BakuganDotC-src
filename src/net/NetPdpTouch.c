// bdc 0x089d2fbc NetPdpTouch
#include "bdc.h"

/* Records the current time as the last packet time (`*0x08ac5978 + 0x14`) and clears the `CONetPDP`
   timeout bytes `+0x10..+0x12`. Called by `NetCharaReceivePacket` for a new sender. */

void NetPdpTouch(NetPdp *self)

{
  sceRtcGetCurrentClockLocalTime(&g_netPdpState->lastRecvTime);
  self->timedOut = '\0';
  self->shortTimedOut = '\0';
  self->packetReceived = '\0';
  return;
}

