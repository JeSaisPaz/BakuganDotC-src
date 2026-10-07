// bdc 0x089c2b08 SndBgmPlayerGet
#include "bdc.h"

/* Returns the `SndBgmPlayer` of slot `index` in `g_soundBgmPlayers`
   (`g_soundBgmPlayers[index]`), NULL if it was not created. There is no range check, so callers
   must pass 0 or 1 (`SndBgmPlayerExists` is the checked guard). */

SndBgmPlayer *SndBgmPlayerGet(s32 index)

{
  return g_soundBgmPlayers[index];
}

