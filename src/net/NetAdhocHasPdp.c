// bdc 0x089d4cf4 NetAdhocHasPdp
#include "bdc.h"

/* Returns whether the PDP socket object exists (`NetPdpExists`). */

bool NetAdhocHasPdp(NetAdhocConn *self)
{
  return NetPdpExists() != 0;
}
