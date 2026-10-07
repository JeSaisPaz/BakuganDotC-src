// bdc 0x089c59b0 SndGetManager
#include "bdc.h"

/* Returns `g_soundManager`, the sound manager singleton (US decomp: `Get_DAT_08AC5874`). */

SndManager *SndGetManager(void)

{
  return g_soundManager;
}

