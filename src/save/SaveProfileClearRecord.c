// bdc 0x0880cddc SaveProfileClearRecord
#include "bdc.h"

/* Clears record `n` of the profile's record table (`SaveProfileGetRecordTable`): `n == 0`
   zero-fills the whole 0x1e0-byte table, `n` in 1..20 zero-fills the 8-byte entry `n-1` in each of
   the three parallel arrays (`+0x00`, `+0xa0`, `+0x140`). Called by `SaveProfileReset` with 0. */

void SaveProfileClearRecord(SaveProfile *self, s32 n)
{
  SaveRecordTable *table;
  s32 k;

  table = (SaveRecordTable *)SaveProfileGetRecordTable(self);
  if (table != NULL) {
    k = n - 1;
    if (n == 0) {
      memset(table, 0, 0x1e0);
    } else if (k >= 0 && k < 0x14) {
      memset(table->a[k], 0, 8);
      memset(table->c[k], 0, 8);
      memset(table->b[k], 0, 8);
    }
  }
}
