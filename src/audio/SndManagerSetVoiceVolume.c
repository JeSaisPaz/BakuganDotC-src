// bdc 0x089c5d6c SndManagerSetVoiceVolume
#include "bdc.h"

/* Stores the voice-stream volume factor (0..1) at `SndManager + 0x8bdc` and, if decoder channel 1
   exists, applies it as that decoder's base volume with `SndDecOutSetBaseVolume` (channel 1 is
   the streamed-voice channel). */

void SndManagerSetVoiceVolume(float volume, SndManager *mgr)

{
  s32 exists;
  SndDecOut *dec;
  
  mgr->voiceVolume = volume;
  exists = SndDecOutExists(1);
  if (exists != 0) {
    dec = SndDecOutGet(1);
    SndDecOutSetBaseVolume(volume,dec);
  }
  return;
}

