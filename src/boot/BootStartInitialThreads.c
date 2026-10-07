// bdc 0x089bbbb0 BootStartInitialThreads
#include "bdc.h"

/* Starts the seven threads listed in `g_bootStartList` by calling `BootStartThread` for each
   index with no argument block. Returns 1 if all started, 0 on the first failure. */

int BootStartInitialThreads(void)
{
  int ok;
  s32 *entry;
  int count;

  count = 0;
  entry = g_bootStartList;
  do {
    ok = BootStartThread(*entry,(void *)0x0,0);
    count = count + 1;
    if (ok == 0) {
      return 0;
    }
    entry = entry + 1;
  } while (count < 7);
  return 1;
}
