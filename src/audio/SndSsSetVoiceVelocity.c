// bdc 0x08a218b4 SndSsSetVoiceVelocity
#include "bdc.h"

/* Stores the note velocity of voice `voice` in `g_sndSsVoiceVelocity`. */

void SndSsSetVoiceVelocity(s32 voice, u8 velocity)

{
  g_sndSsVoiceVelocity[voice] = velocity;
}
