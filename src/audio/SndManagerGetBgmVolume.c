// bdc 0x089c5d5c SndManagerGetBgmVolume
#include "bdc.h"

/* Returns the BGM volume factor (0..1) stored by `SndManagerSetBgmVolume` at `SndManager +
   0x8bd8`. */

float SndManagerGetBgmVolume(SndManager *mgr)

{
  return mgr->bgmVolumeF;
}

