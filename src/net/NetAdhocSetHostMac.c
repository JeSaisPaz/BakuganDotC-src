// bdc 0x089d3d70 NetAdhocSetHostMac
#include "bdc.h"

/* Copies the 6-byte host MAC to the connection object `+0x3c` (zeroes it for NULL). The host MAC
   also names the game-mode adhocctl group (`NetAdhocBuildGroupName`) and is the send target of
   clients in `NetPdpUpdate`. */

void NetAdhocSetHostMac(NetAdhocConn *self, const u8 *mac)

{
  if (mac != (u8 *)0x0) {
    memcpy(self->hostMac,mac,6);
    return;
  }
  memset(self->hostMac,0,6);
  return;
}

