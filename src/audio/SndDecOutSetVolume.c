// bdc 0x089c4ce4 SndDecOutSetVolume
#include "bdc.h"

/* Starts a linear fade of the decoder's volume: `percent` is clamped to 0..100 and becomes `volTo`
   while the previous target becomes `volFrom`; the fade length is `fadeMs × 44100 / 1000 /
   blockFrames` output blocks (`blockFrames` = first dword of `SndManager`, 0x100) stored in
   `fadeLeft`/`fadeTotal`. With a zero-length fade both values are set to `percent` immediately. */

void SndDecOutSetVolume(SndDecOut *dec, s32 percent, s32 fadeMs)

{
  s32 blocks;
  
  CoreLockAcquire(dec->lock);
  if (percent < 0) {
    percent = 0;
  }
  if (100 < percent) {
    percent = 100;
  }
  dec->volFrom = dec->volTo;
  dec->volTo = percent;
  blocks = ((fadeMs * 0xac44) / 1000) / SndGetManager()->framesPerBlock;
  dec->fadeTotal = blocks;
  dec->fadeLeft = blocks;
  if (blocks == 0) {
    dec->volTo = percent;
    dec->volFrom = percent;
  }
  CoreLockRelease(dec->lock);
  return;
}

