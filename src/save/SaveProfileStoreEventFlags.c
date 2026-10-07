// bdc 0x088c2978 SaveProfileStoreEventFlags
#include "bdc.h"

/* Snapshots the live story event state into the player profile: copies the 0x108-byte live
   event-flag block `g_gameEventFlags` into the save block at `data->eventFlags`, copies the 16
   bytes of `g_gameUnlockFlags` to `data->unlockFlags`, sets the byte
   `eventFlagsStored` to 1 and then calls `GameFieldSyncProgressFlags` to refresh the unlock bitmaps from
   the flags. Called by `ScriptOpStartNextPlaythrough` after the flag reset and by three other
   callers. */

void SaveProfileStoreEventFlags(void)
{
  u32 *dst;
  const u32 *src;
  int i;

  dst = (u32 *)SaveGetProfile()->data->eventFlags;
  src = (const u32 *)g_gameEventFlags;
  for (i = 0; i < 0x21; i++) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst += 2;
    src += 2;
  }
  dst = (u32 *)SaveGetProfile()->data->unlockFlags;
  src = (const u32 *)g_gameUnlockFlags;
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
  SaveGetProfile()->data->eventFlagsStored = 1;
  GameFieldSyncProgressFlags();
}
