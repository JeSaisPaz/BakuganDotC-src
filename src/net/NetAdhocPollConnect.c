// bdc 0x089d3ed4 NetAdhocPollConnect
#include "bdc.h"

/* Polls a connect request: returns 1 once the link state is 1 (connected), -1 when the link fell
   back to idle (`NetAdhocIsDisconnected`), 0 while still in progress. */

s32 NetAdhocPollConnect(NetAdhocConn *self)

{
  s32 result = NetAdhocLinkStateIs(self, 1) ? 1 : 0;

  if (result == 0 && NetAdhocIsDisconnected(self)) {
    result = -1;
  }
  return result;
}

