// bdc 0x089c37a8 SndBgmPlayerStopF
#include "bdc.h"

/* Float wrapper of `SndBgmPlayerStop`: `fadeSec × 1000` milliseconds. */

s32 SndBgmPlayerStopF(float fadeSec, SndBgmPlayer *player, u8 keepPath)

{
  return SndBgmPlayerStop(player,(int)(fadeSec * 1000.0f),keepPath);
}
