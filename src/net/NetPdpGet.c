// bdc 0x089d2e58 NetPdpGet
#include "bdc.h"

/* Returns the `CONetPDP` object (`**0x08ac5978`); no NULL check. */

void *NetPdpGet(void)

{
  return g_netPdpState->pdp;
}

