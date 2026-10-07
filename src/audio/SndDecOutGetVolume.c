// bdc 0x089c4dd8 SndDecOutGetVolume
#include "bdc.h"

/* Returns the decoder's effective volume for the next block: `baseVolume × percent / 100`, where
   `percent` interpolates linearly from `volFrom` to `volTo` over `fadeTotal` blocks (`fadeLeft`
   remaining); once the fade is over `volFrom` is set to `volTo`. In modes 2 and 6 (movie audio)
   only `baseVolume` is returned. */

float SndDecOutGetVolume(SndDecOut *dec)

{
  int pct;
  float vol;
  
  CoreLockAcquire(dec->lock);
  pct = dec->volTo;
  vol = dec->baseVolume;
  if (dec->fadeLeft < 1) {
    dec->volFrom = pct;
    dec->fadeLeft = 0;
  }
  else {
    pct = ((((dec->fadeTotal - dec->fadeLeft) * 100) / dec->fadeTotal) * (pct - dec->volFrom)) /
            100 + dec->volFrom;
  }
  if ((dec->mode != 2) && (dec->mode != 6)) {
    vol = vol * (float)pct * 0.01f;
  }
  CoreLockRelease(dec->lock);
  return vol;
}

