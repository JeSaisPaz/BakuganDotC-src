// bdc 0x088f52a0 GameEventFlagSet
#include "bdc.h"

/* Sets story event flag `id` (16-bit, valid below 2000): counter-class ids (see
   `GameEventFlagTest`) increment the shared counter byte `g_gameEventFlags[5]`, saturating at 99;
   for other ids, unless `GameEventFlagIs3dbOr3dc(id)` reports it, either redirect through
   `GameEventFlagIsSpecialSet394(id)` (when non-zero it calls
   `GameEventFlagSyncUpgrade(GameEventFlagRangeIndex(5, id))` instead) or set bit `id & 31` of word
   `id >> 5` of the bitset at `g_gameEventFlags[8]`. */

void GameEventFlagSet(u32 id)

{
  u32 i = id & 0xffff;
  u32 *bits;
  s32 n;

  if ((s32)i < 2000) {
    if (GameEventFlagInRange(2, (u16)id) != 0) {
      n = g_gameEventFlags[5] + 1;
      g_gameEventFlags[5] = (n < 100) ? n : 99;
      return;
    }
    if (GameEventFlagIs3dbOr3dc((s16)i) == 0) {
      if (GameEventFlagIsSpecialSet394((s16)i) != 0) {
        GameEventFlagSyncUpgrade(GameEventFlagRangeIndex(5, (s16)i));
        return;
      }
      bits = (u32 *)&g_gameEventFlags[8];
      bits[(s32)i >> 5] |= 1 << (i & 0x1f);
    }
  }
}
