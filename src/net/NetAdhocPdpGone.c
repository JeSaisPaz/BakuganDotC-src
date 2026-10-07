// bdc 0x089d4f98 NetAdhocPdpGone
#include "bdc.h"

/* Returns 1 when the PDP object no longer exists (`NetPdpExists` == 0). */

bool NetAdhocPdpGone(NetAdhocConn *self)

{
  bool gone;
  
  gone = false;
  if (self->mode == 1) {
    if (NetPdpExists() == 0) {
      gone = true;
    }
  }
  else {
    if (NetPdpExists() == 0) {
      gone = true;
    }
  }
  return gone;
}

