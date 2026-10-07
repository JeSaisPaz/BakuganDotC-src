// bdc 0x089c4d98 SndDecOutSetVolumeF
#include "bdc.h"

/* Float wrapper of `SndDecOutSetVolume`: `volume` (0..1) × 100 percent and `fadeSec` × 1000 ms,
   truncated to int. */

void SndDecOutSetVolumeF(float volume, float fadeSec, SndDecOut *dec)

{
  SndDecOutSetVolume(dec,(int)(volume * 100.0f),(int)(fadeSec * 1000.0f));
  return;
}

