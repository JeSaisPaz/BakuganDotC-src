// bdc 0x0880c800 SaveGetDataBlockSize
#include "bdc.h"

/* Returns the size of the save data / script variable block (`ScriptVarsGetBlockSize()`, which returns
   0xfd0). `SaveProfileReset` stores `size + 2` in profile word `+4` as a format stamp, and
   `SaveProfileIsValid` checks it. */

s32 SaveGetDataBlockSize(void)
{
  return ScriptVarsGetBlockSize();
}
