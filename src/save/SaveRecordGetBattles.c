// bdc 0x0880d8bc SaveRecordGetBattles
#include "bdc.h"

/* Returns the battle count of Bakugan `bakugan` (1..20) in mode `mode` (0..2) from the
   battle-record table (`SaveProfileGetRecordTable`: 3 modes × 20 Bakugan × 8 bytes, entries
   `{s16 battles, s16 wins, s16 losses, s16 pad}`), or -1 for a bad id or missing table. */

s32 SaveRecordGetBattles(SaveProfile *self, s32 bakugan, s32 mode)

{
  s16 *table;
  s32 index;
  s32 result;

  result = -1;
  table = (s16 *)SaveProfileGetRecordTable(self);
  index = bakugan - 1;
  if (table != NULL && index >= 0 && index < 20) {
    result = table[mode * 80 + index * 4 + 0];
  }
  return result;
}
