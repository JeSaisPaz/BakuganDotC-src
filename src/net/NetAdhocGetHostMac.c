// bdc 0x089d3dc4 NetAdhocGetHostMac
#include "bdc.h"

/* Returns the address of the host MAC field of the connection object. */
u8 *NetAdhocGetHostMac(NetAdhocConn *self)
{
    return self->hostMac;
}
