// bdc 0x089c5b2c SndManagerSetMasterVolume
#include "bdc.h"

/* Sets the master volume `volume` (0..1): scales it to 0..127 (`volume × 127`, clamped) and, if
   the audio-settings block (`g_soundAudioSettings`) exists, stores the byte at its `masterVolume`
   (`+4`) and `masterVolume2` (`+6`) under the manager lock; also keeps the float
   `SndManager.masterVolume` for `SndManagerGetMasterVolumeF`. */

void SndManagerSetMasterVolume(float volume, SndManager *mgr)
{
  int v;

  v = (int)(volume * 127.0f);
  if (v < 0) {
    v = 0;
  }
  if (0x7f < v) {
    v = 0x7f;
  }
  if (g_soundAudioSettings != (SndAudioSettings *)0x0) {
    CoreLockAcquire(mgr->lock);
    g_soundAudioSettings->masterVolume = (u8)v;
    g_soundAudioSettings->masterVolume2 = (u8)v;
    CoreLockRelease(mgr->lock);
  }
  mgr->masterVolume = volume;
}
