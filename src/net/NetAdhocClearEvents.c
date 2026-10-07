// bdc 0x089d4950 NetAdhocClearEvents
#include "bdc.h"

/* Clears the adhocctl event mask `+0xc` and the last error `+0x8` of the connection object. */

void NetAdhocClearEvents(NetAdhocConn *self)

{
  self->eventMask = 0;
  self->lastError = 0;
  return;
}

