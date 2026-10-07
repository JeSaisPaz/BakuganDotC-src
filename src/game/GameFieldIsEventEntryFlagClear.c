// bdc 0x088bf5f4 GameFieldIsEventEntryFlagClear
#include "bdc.h"

/* Returns 1 when the entry's flag `+4` (`GameFieldGetEventEntry`) is 0 or not set
   (`GameEventFlagTest`). */

s32 GameFieldIsEventEntryFlagClear(CoreTask *task, u16 id)
{
  u16 *entry = (u16 *)GameFieldGetEventEntry(task, id);
  u16 flag = entry[2];

  if (flag == 0) {
    return 1;
  }
  if (!GameEventFlagTest(flag)) {
    return 1;
  }
  return 0;
}
