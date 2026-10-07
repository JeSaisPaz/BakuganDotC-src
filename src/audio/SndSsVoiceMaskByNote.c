// bdc 0x08a22f6c SndSsVoiceMaskByNote
#include "bdc.h"

/* Returns the mask of sounding voices (state 3 or 7) playing note `chanNote[1]` on channel
   `chanNote[0]` for `handle`. */

u32 SndSsVoiceMaskByNote(const u8 *chanNote, s32 handle)

{
  u32 mask = 0;
  u32 bit = 1;
  u32 i;

  if (g_sndSsVoiceState != -1 && chanNote != NULL) {
    for (i = 0; i < g_sndSsVoiceCount; i++) {
      SndSsVoice *v = &g_sndSsVoices[i];
      if ((v->state == 3 || v->state == 7) && v->channel == chanNote[0] && v->note == chanNote[1] &&
          v->handle == handle) {
        mask |= bit;
      }
      bit <<= 1;
    }
  }
  return mask;
}
