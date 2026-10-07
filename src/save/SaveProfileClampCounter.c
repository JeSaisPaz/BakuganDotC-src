// bdc 0x0880d514 SaveProfileClampCounter
#include "bdc.h"

/* Clamps `*value` into `[SaveProfileCounterMin, SaveProfileCounterMax]` for counter `idx` of
   `profile`, in place. */

void SaveProfileClampCounter(SaveProfile *self, s32 idx, s32 *value)
{
  s32 max;
  s32 min;
  s32 v;

  max = SaveProfileCounterMax(self, idx);
  min = SaveProfileCounterMin(self, idx);
  v = *value;
  if (v < min) {
    *value = min;
    return;
  }
  if (max < v) {
    *value = max;
    return;
  }
  *value = v;
}
