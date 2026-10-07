// bdc 0x089c3340 SndBgmPlayerSetVolumeF
#include "bdc.h"

/* Float wrapper of `SndBgmPlayerSetVolume`: `volume` (0..1) becomes percent (`× 100`) and
   `fadeSec` milliseconds (`× 1000`), both truncated to int. The two floats arrive in FPU
   registers, the player in `a0`. */

void SndBgmPlayerSetVolumeF(float volume, float fadeSec, SndBgmPlayer *player)

{
  SndBgmPlayerSetVolume(player,(int)(volume * 100.0f),(int)(fadeSec * 1000.0f));
  return;
}

