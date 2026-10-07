// bdc 0x089c4ca4 SndDecOutSetBaseVolume
#include "bdc.h"

/* Sets the decoder's base volume (`baseVolume`, 0..1 factor multiplied with the fade percentage in
   `SndDecOutGetVolume`) under its lock. `volume` arrives in an FPU register, `dec` in `a0`. */

void SndDecOutSetBaseVolume(float volume, SndDecOut *dec)

{
  CoreLockAcquire(dec->lock);
  dec->baseVolume = volume;
  CoreLockRelease(dec->lock);
  return;
}

