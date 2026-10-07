// bdc 0x089d36b0 NetAdhocctlHandler
#include "bdc.h"

/* `sceNetAdhocctl` event handler registered by `NetAdhocInitStep` (`sceNetAdhocctlAddHandler`):
   if the ad-hoc manager exists it forwards `(event, error)` to `NetAdhocOnCtlEvent` on the
   COPSPNet connection object. */

void NetAdhocctlHandler(s32 event, s32 error, void *arg)

{
  if (NetAdhocHasManager()) {
    NetAdhocOnCtlEvent(NetAdhocGetManager(), event, error);
  }
  return;
}

