// bdc 0x089d2990 NetInviteDestroy
#include "bdc.h"

/* Destroys the ad-hoc invitation singleton `g_netInvite`: runs `NetInviteDtor``(obj, 3)` and
   frees the holder. */

void NetInviteDestroy(void)

{
  void **holder;
  
  if (g_netInvite != (void **)0x0) {
    holder = g_netInvite;
    if (*g_netInvite != (void *)0x0) {
      NetInviteDtor(*g_netInvite,3);
      holder = g_netInvite;
      *g_netInvite = (void *)0x0;
    }
    if (holder != (void **)0x0) {
      MemLock();
      MemFree(g_netInvite,(char *)0x0,0);
      MemUnlock();
      g_netInvite = (void **)0x0;
    }
  }
  return;
}

