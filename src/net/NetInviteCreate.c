// bdc 0x089d28a0 NetInviteCreate
#include "bdc.h"

/* Creates the ad-hoc invitation singleton `g_netInvite`: allocates the zeroed 4-byte holder and
   the 0x78-byte `CONetInvate` object (`NetInviteCtor`). Called from the net-chara start-up
   `NetCharaMgrCreate`. */

void NetInviteCreate(void)

{
  bool wasLow;
  void **s;
  void *invite;
  void *result;
  
  if (g_netInvite == (void **)0x0) {
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    s = MemAlloc(sizeof(void *),(char *)0x0,0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    g_netInvite = s;
    memset(s,0,sizeof(void *));
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    invite = MemAlloc(sizeof(NetInvite),(char *)0x0,0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    result = (void *)0x0;
    if (invite != (void *)0x0) {
      NetInviteCtor(invite);
      result = invite;
    }
    *g_netInvite = result;
  }
  return;
}

