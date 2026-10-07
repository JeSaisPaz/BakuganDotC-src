// bdc 0x089d4ba0 NetAdhocHasConnectedEvent
#include "bdc.h"

/* Returns whether the adhocctl connect event (1) has arrived (`NetAdhocHasEvent`). */

bool NetAdhocHasConnectedEvent(NetAdhocConn *self)
{
  return NetAdhocHasEvent(self, 1) != 0;
}
