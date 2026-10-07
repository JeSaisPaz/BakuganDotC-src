// bdc 0x089cc738 SysUtilSavedataIsDone
#include "bdc.h"

/* Returns whether the current savedata request has finished: the busy word of the savedata
   parameter block `g_savedataParams` is 0. Polled every frame by the `Save*Task` classes after
   starting a request. */

bool SysUtilSavedataIsDone(void)

{
  return g_savedataParams->busy == 0;
}
