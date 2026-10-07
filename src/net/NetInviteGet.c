// bdc 0x089d2a20 NetInviteGet
#include "bdc.h"

/* Returns the `CONetInvate` object held by `g_netInvite` (no NULL check; see `NetHasInvite`).
    */

void *NetInviteGet(void)

{
  return *g_netInvite;
}

