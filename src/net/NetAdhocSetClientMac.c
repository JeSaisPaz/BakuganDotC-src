// bdc 0x089d3dcc NetAdhocSetClientMac
#include "bdc.h"

/* Copies the 6-byte client MAC to the connection object `+0x42` (zeroes it for NULL); the host
   sends to this address in `NetPdpUpdate`. */

void NetAdhocSetClientMac(NetAdhocConn *self, const u8 *mac)

{
  if (mac != (u8 *)0x0) {
    memcpy(self->clientMac,mac,6);
    return;
  }
  memset(self->clientMac,0,6);
  return;
}

