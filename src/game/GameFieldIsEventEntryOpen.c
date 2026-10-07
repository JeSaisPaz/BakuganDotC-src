// bdc 0x088bf4f0 GameFieldIsEventEntryOpen
#include "bdc.h"

/* Returns 1 when neither event flag of entry `id` (`GameFieldGetEventEntry`: the u16 at `+4`,
   then the one at `+2`) is set in the story bitset `g_gameEventFlags + 8` (a zero flag id counts
   as clear); returns 0 as soon as one of them is set. */

s32 GameFieldIsEventEntryOpen(CoreTask *task, u16 id)
{
  u16 *entry;
  u32 *bits;
  s32 flag;
  u32 set;

  bits = (u32 *)&g_gameEventFlags[8];
  entry = (u16 *)GameFieldGetEventEntry(task, id);
  set = 0;
  flag = entry[2];
  if (flag != 0) {
    set = (bits[flag / 32] >> (flag % 32)) & 1;
  }
  if (set == 0) {
    flag = entry[1];
    set = 0;
    if (flag != 0) {
      set = (bits[flag / 32] >> (flag % 32)) & 1;
    }
    if (set == 0) {
      return 1;
    }
  }
  return 0;
}
