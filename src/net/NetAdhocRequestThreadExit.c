// bdc 0x089d3abc NetAdhocRequestThreadExit
#include "bdc.h"

/* Sets the byte `0x08ac5994` that tells the net thread `BootNetworkThread` to exit once the link
   is stopped; set by step 3 of `NetPlayStateAbort`. */

void NetAdhocRequestThreadExit(void)

{
  g_netAdhocThreadExit = true;
  return;
}

