// bdc 0x089c6964 SndManagerFadeOutAllVoices
#include "bdc.h"

/* Starts a short fade-out on every live voice of the `SndManager`: under the manager lock it
   walks the 32 voice slots and calls `SndManagerStartVoiceFadeOut` with the handle of each slot
   whose `handle != 0`. The fade itself (0.125 s) is run by `SndManagerUpdateVoices`, which stops
   the voice at the end. */

void SndManagerFadeOutAllVoices(SndManager *mgr)

{
  int i;
  int handle;

  CoreLockAcquire(mgr->lock);
  for (i = 0; i < 32; i++) {
    handle = mgr->voices[i].handle;
    if (handle != 0) {
      SndManagerStartVoiceFadeOut(mgr, handle);
    }
  }
  CoreLockRelease(mgr->lock);
  return;
}
