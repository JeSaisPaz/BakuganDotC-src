// bdc 0x088f501c GameEventFlagTest
#include "bdc.h"

/* Tests story event flag `id` (16-bit, valid below 2000): for counter-class ids (inside the range
   `GameEventFlagInRange(2, id)` checks against the tables at `0x08a99800` / `0x08a99810`) it returns
   whether the shared counter byte at `0x08b00bd9` is non-zero, otherwise it tests bit `id & 31` of
   word `id >> 5` of the bitset at `0x08b00bdc`. Returns false for ids >= 2000. About 10 callers,
   e.g. `GameEventFlagsAdvanceStage` and `GameFieldSyncProgressFlags` (which asks for flag `0x215`). */

bool GameEventFlagTest(u32 id)
{
  u32 *bits;

  id &= 0xffff;
  if (id < 2000) {
    if (GameEventFlagInRange(2, (u16)id) != 0) {
      if (g_gameEventFlags[5] != 0) {
        return true;
      }
    } else {
      bits = (u32 *)&g_gameEventFlags[8];
      if (((bits[(s32)id >> 5] >> (id & 0x1f)) & 1) != 0) {
        return true;
      }
    }
  }
  return false;
}
