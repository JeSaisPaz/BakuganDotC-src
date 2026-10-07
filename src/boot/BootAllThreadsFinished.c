// bdc 0x089bbc1c BootAllThreadsFinished
#include "bdc.h"

/* Returns 1 when none of the seven threads listed in `g_bootStartList` is running any more (all
   `BootIsThreadRunning` checks are 0), else 0. `main` calls it at the top of every service-loop
   iteration and leaves the loop once the game threads have all ended. */

int BootAllThreadsFinished(void)
{
  int allFinished = 1;
  int i = 0;
  s32 *entry = g_bootStartList;

  do {
    if (BootIsThreadRunning(*entry) != 0) {
      allFinished = 0;
    }
    i++;
    entry++;
  } while (i < 7);
  return allFinished;
}
