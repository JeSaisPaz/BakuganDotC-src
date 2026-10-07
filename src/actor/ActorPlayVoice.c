// bdc 0x088df9b0 ActorPlayVoice
#include "bdc.h"

/* Plays voice line `voiceId` (`SndBgmPlayVoice`) when it is not negative; the actor argument is
   unused. */

void ActorPlayVoice(void *actor, s16 voiceId)

{
  if (-1 < voiceId) {
    SndBgmPlayVoice((int)voiceId);
  }
  return;
}

