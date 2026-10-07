// bdc 0x089c5cbc SndManagerSetBgmVolume
#include "bdc.h"

/* Sets the BGM volume factor `volume` (0..1): stores `clamp(volume × 4096, 0, 0x1000)` in
   `SndManager + 0x10`, the float in `SndManager + 0x8bd8` (read back by
   `SndManagerGetBgmVolume`), and applies it as the base volume (`SndDecOutSetBaseVolume`) of
   decoder channels 0 and 1 when they exist. */

void SndManagerSetBgmVolume(float volume, SndManager *mgr)

{
  SndDecOut *dec;
  s32 v;
  
  v = (s32)(volume * 4096.0f);
  if (v < 0) {
    v = 0;
  }
  if (0x1000 < v) {
    v = 0x1000;
  }
  mgr->bgmVolume = v;
  mgr->bgmVolumeF = volume;
  v = SndDecOutExists(0);
  if (v != 0) {
    dec = SndDecOutGet(0);
    SndDecOutSetBaseVolume(volume,dec);
  }
  v = SndDecOutExists(1);
  if (v != 0) {
    dec = SndDecOutGet(1);
    SndDecOutSetBaseVolume(volume,dec);
  }
  return;
}

