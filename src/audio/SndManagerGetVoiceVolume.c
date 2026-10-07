// bdc 0x089c5dbc SndManagerGetVoiceVolume
#include "bdc.h"

/* Returns the voice-stream volume factor (0..1) stored by `SndManagerSetVoiceVolume` (`SndManager
   + 0x8bdc`). */

float SndManagerGetVoiceVolume(SndManager *mgr)

{
  return mgr->voiceVolume;
}

