// bdc 0x089d2e30 NetPdpExists
#include "bdc.h"

/* Returns 1 when the PDP holder `*0x08ac5978` and its `CONetPDP` object exist, else 0. */

int NetPdpExists(void)

{
  if (g_netPdpState != (NetPdpState *)0x0 && g_netPdpState->pdp != (NetPdp *)0x0) {
    return 1;
  }
  return 0;
}
