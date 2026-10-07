// bdc 0x089d4f08 NetAdhocFlushPdp
#include "bdc.h"

/* When the PDP object exists, closes it (`NetPdpRequestClose`) and runs one more PDP update
   (`NetPdpStep`) — step 0 of `NetAdhocTermStep`. */

void NetAdhocFlushPdp(NetAdhocConn *self)

{
  if (self->mode == 1) {
    if (NetPdpExists()) {
      NetPdpRequestClose(NetPdpGet());
      NetPdpStep(NetPdpGet());
      return;
    }
  }
  else {
    if (NetPdpExists()) {
      NetPdpRequestClose(NetPdpGet());
      NetPdpStep(NetPdpGet());
    }
  }
  return;
}

