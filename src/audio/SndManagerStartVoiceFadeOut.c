// bdc 0x089c6af8 SndManagerStartVoiceFadeOut
#include "bdc.h"

/* Finds the voice whose `handle` equals `handle` and sets its fade timer (`SndVoiceSlot +0x10`,
   float) to 0.125 s (`0x3e000000`). `SndManagerUpdateVoices` then ramps the voice volume down
   linearly over that time and stops the voice. Does nothing when no voice has that handle. The
   caller holds the manager lock (`SndManagerFadeOutAllVoices`). */

void SndManagerStartVoiceFadeOut(SndManager *mgr, s32 handle)

{
  s32 i;

  for (i = 0; i < 0x20; i++) {
    if (mgr->voices[i].handle == handle) {
      mgr->voices[i].fadeTimer = 0.125f;
      return;
    }
  }
}
