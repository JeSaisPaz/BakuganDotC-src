// bdc 0x089d419c NetAdhocIsHost
#include "bdc.h"

/* Returns the host flag byte `+0x58` of the connection object. */
bool NetAdhocIsHost(NetAdhocConn *self)
{
    return self->isHost;
}
