// bdc 0x089da504 GmoMotionEntryReserveArena
#include "bdc.h"

/* Virtual method (vtable entry 6): allocates `size` bytes from the low end of the heap as the
   entry's track-data `arena`, records the size in `arenaSize` and resets the bump offset
   `g_gmoArenaOffset` to 0. The allocation result is stored unchecked (NULL on failure). */

void GmoMotionEntryReserveArena(GmoMotionEntry *entry, s32 size)
{
  bool fromLow;
  u8 *arena;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  arena = MemAlloc(size, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  entry->arena = arena;
  entry->arenaSize = size;
  g_gmoArenaOffset = 0;
}
