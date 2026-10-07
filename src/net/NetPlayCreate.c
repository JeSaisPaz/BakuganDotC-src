// bdc 0x0881afd8 NetPlayCreate
#include "bdc.h"

/* Starts ad-hoc play: creates the NetPlay task 0x7d2 at priority 100 (`CoreTaskCreate`) and, if
   `g_netPlay` is NULL, allocates the 4-byte holder and the 0xf0-byte manager (`NetPlayCtor`).
   Called by `UiNetLobbyHostPhase` and `UiNetLobbyJoinPhase`; undone by `NetPlayShutdown`. */

void NetPlayCreate(void)

{
  bool prevLow;
  void **holder;
  NetPlay *self;
  NetPlay *obj;
  
  CoreTaskCreate(0x7d2,100);
  if (g_netPlay == (void **)0x0) {
    MemLock();
    prevLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    holder = MemAlloc(4,(char *)0x0,0);
    MemSetAllocFromLow(prevLow);
    MemUnlock();
    g_netPlay = holder;
    MemLock();
    prevLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    self = MemAlloc(0xf0,(char *)0x0,0);
    MemSetAllocFromLow(prevLow);
    MemUnlock();
    obj = (NetPlay *)0x0;
    if (self != (NetPlay *)0x0) {
      NetPlayCtor(self);
      obj = self;
    }
    *g_netPlay = obj;
  }
  return;
}

