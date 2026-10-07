// bdc 0x089da584 GmoMotionEntryArenaAlloc
#include "bdc.h"

/* Virtual method (vtable entry 7): bump allocator over the entry's arena. Returns the next 4-byte
   aligned address after the offset `g_gmoArenaOffset` and advances the offset, or NULL when there is no
   arena or it is full (`size + offset > arenaSize`). */

void *GmoMotionEntryArenaAlloc(GmoMotionEntry *entry, s32 size)

{
  void *result;
  u8 *cur;

  result = (void *)0x0;
  if (entry->arena != (u8 *)0x0) {
    cur = entry->arena + g_gmoArenaOffset;
    if (size + g_gmoArenaOffset <= entry->arenaSize) {
      result = (void *)(((uintptr_t)(cur + 3)) & ~(uintptr_t)3);
      g_gmoArenaOffset = g_gmoArenaOffset + size + (s32)((u8 *)result - cur);
    }
  }
  return result;
}
