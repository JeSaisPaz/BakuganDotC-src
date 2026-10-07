// bdc 0x089c4b54 SndDecOutRequestResume
#include "bdc.h"

/* Ends a decoder pause: sets `resumeRequested = 1` and, if the decoder thread (slot channel + 6) is
   sleeping (`BootIsThreadSleeping`), wakes it (`BootWakeupThread`). The thread then clears
   `pauseRequested`, `paused` and `resumeRequested`. */

void SndDecOutRequestResume(SndDecOut *dec)

{
  
  dec->resumeRequested = 1;
  if (BootIsThreadSleeping(dec->channel + 6)) {
    BootWakeupThread(dec->channel + 6);
  }
  return;
}

