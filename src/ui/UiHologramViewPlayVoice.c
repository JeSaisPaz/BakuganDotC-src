// bdc 0x0892983c UiHologramViewPlayVoice
#include "bdc.h"

/* Plays the narration voice for page `page` of the hologram detail view (`UiHologramViewCtor`,
   task 392; view kind `+0x485`): looks it up in the table `0x08ac1338` (36 ints), stores it in
   `+0x70c` and plays it with `SndBgmPlayVoice` unless -1. */

void UiHologramViewPlayVoice(UiHologramView *self, u32 page)

{
  int voiceId;
  int voices [36];
  
  memcpy(voices,g_hologramViewVoiceTable,0x90);
  voiceId = voices[page & 0xff];
  self->voice = voiceId;
  if (voiceId != -1) {
    SndBgmPlayVoice(voiceId);
  }
  return;
}

