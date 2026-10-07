// bdc 0x089d3e20 NetAdhocGetClientMac
#include "bdc.h"

/* Returns the address of the client MAC field (`+0x42`) of the connection object. */
u8 *NetAdhocGetClientMac(NetAdhocConn *self)
{
    return self->clientMac;
}
