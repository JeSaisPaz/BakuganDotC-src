// bdc 0x089c5bdc SndManagerGetMasterVolume
#include "bdc.h"

/* Returns the master volume limit read from the audio settings object at `g_soundAudioSettings`
   (`u8` at `+4`), taking the manager lock around the read; returns 0 if the settings object does
   not exist. */

u8 SndManagerGetMasterVolume(SndManager *mgr)

{
  u8 result;

  result = 0;
  if (g_soundAudioSettings != NULL) {
    CoreLockAcquire(mgr->lock);
    result = g_soundAudioSettings->masterVolume;
    CoreLockRelease(mgr->lock);
  }
  return result;
}
