// bdc 0x08a20c8c SndSsGetVoiceStatus
#include "bdc.h"

/* Returns bit `voiceId` of the voice status mask `SndSasGetEndFlag()` of the Sony sound layer (0 or
   1), or `0x80450001` when the layer is not initialised. `SndManagerUpdateVoices` treats a voice
   as still sounding when the bit is 0 and as ended otherwise. */

u32 SndSsGetVoiceStatus(u32 voiceId)

{
  u32 status;
  
  status = 0x80450001;
  if (g_sndSsState != -1) {
    status = SndSasGetEndFlag();
    status = status >> (voiceId & 0x1f) & 1;
  }
  return status;
}

