// bdc 0x089c4b14 SndDecOutRequestPause
#include "bdc.h"

/* Starts the suspend handshake on the decoder side: under the lock sets `pauseRequested = 1` and
   clears `paused`. `SndDecOutThreadStep` then drains the hardware channel, clears its buffers and
   sets `paused`. */

void SndDecOutRequestPause(SndDecOut *dec)

{
  CoreLockAcquire(dec->lock);
  dec->pauseRequested = '\x01';
  dec->paused = '\0';
  CoreLockRelease(dec->lock);
  return;
}

