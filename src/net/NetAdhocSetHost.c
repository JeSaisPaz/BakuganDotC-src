// bdc 0x089d4190 NetAdhocSetHost
#include "bdc.h"

/* Stores the host flag byte `+0x58` of the connection object (set by `NetPlayState1Connect`). */

void NetAdhocSetHost(NetAdhocConn *self, bool isHost)

{
  self->isHost = isHost;
  return;
}

