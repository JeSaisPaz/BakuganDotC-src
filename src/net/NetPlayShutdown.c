// bdc 0x0881b16c NetPlayShutdown
#include "bdc.h"

/* Tears NetPlay down: if the manager exists, destroys the NetPlay object (`NetPlayDtor(obj, 3)`:
   deletes its pad object and "CONetPlay" `CoreLock`, then frees it), clears the holder word,
   frees the holder and sets `g_netPlay` to NULL; finally removes the NetPlay task `0x7d2` with
   `CoreTaskRemoveById`. Only caller: `NetPlayUpdate`, once the object's `finished` byte (`+0xd`) is set
   by `NetPlayStateAbort`. */

void NetPlayShutdown(void)

{
  if (g_netPlay != (void **)0x0) {
    if (*g_netPlay != (NetPlay *)0x0) {
      NetPlayDtor(*g_netPlay,3);
      *g_netPlay = (void *)0x0;
    }
    if (g_netPlay != (void **)0x0) {
      MemLock();
      MemFree(g_netPlay,(char *)0x0,0);
      MemUnlock();
      g_netPlay = (void **)0x0;
    }
  }
  CoreTaskRemoveById(0x7d2);
  return;
}

