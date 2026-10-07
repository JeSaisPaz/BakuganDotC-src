// bdc 0x0880dae0 SaveRecordAddLosses
#include "bdc.h"

/* Adds `delta` to the losses counter (`+4`) of Bakugan `bakugan` in mode `mode` in the
   battle-record table (`SaveProfileGetRecordTable`: 3 modes × 20 Bakugan × 8 bytes, entries
   `{s16 battles, s16 wins, s16 losses, s16 pad}`), clamped to 0..9999; returns the new value or -1.
    */

s32 SaveRecordAddLosses(SaveProfile *self, s32 bakugan, s32 mode, s32 delta)

{
  s16 *table;
  s16 *entry;
  s32 index;
  s32 result;

  result = -1;
  table = (s16 *)SaveProfileGetRecordTable(self);
  index = bakugan - 1;
  if (table != NULL && index >= 0 && index < 20) {
    entry = &table[mode * 80 + index * 4 + 2];
    result = delta + *entry;
    if (result < 0) {
      result = 0;
    }
    if (result > 9999) {
      result = 9999;
    }
    *entry = (s16)result;
  }
  return result;
}
