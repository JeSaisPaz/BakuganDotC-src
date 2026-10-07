// bdc 0x089d3acc NetAdhocIsThreadExitRequested
#include "bdc.h"

/* Returns the exit-request byte `0x08ac5994` (set by `NetAdhocRequestThreadExit`, cleared by
   `NetAdhocConnCtor`); the net thread `BootNetworkThread` leaves its loop when this and
   `NetAdhocIsStopped` are both true. */

bool NetAdhocIsThreadExitRequested(void)

{
  return g_netAdhocThreadExit;
}

