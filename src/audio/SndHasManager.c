// bdc 0x089c5994 SndHasManager
#include "bdc.h"

/* Returns whether `g_soundManager` is non-null (US decomp: `NotZero_DAT_08AC5874`). Guard before
   `SndGetManager` + `SndManagerPlay`. */

bool SndHasManager(void)

{
  return g_soundManager != (SndManager *)0x0;
}

