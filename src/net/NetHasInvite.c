// bdc 0x089d2a04 NetHasInvite
#include "bdc.h"

/* Returns whether `g_netInvite` is non-null (singleton accessor, named by `bdc singleton`). */

bool NetHasInvite(void)

{
  return g_netInvite != (void **)0x0;
}

