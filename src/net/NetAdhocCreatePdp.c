// bdc 0x089d4cd8 NetAdhocCreatePdp
#include "bdc.h"

/* Creates the PDP socket object (`NetPdpCreate`); step 5 of `NetAdhocInitStep`. */

void NetAdhocCreatePdp(NetAdhocConn *self)

{
  NetPdpCreate();
  return;
}

