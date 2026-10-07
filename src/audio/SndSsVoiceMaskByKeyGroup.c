// bdc 0x08a22edc SndSsVoiceMaskByKeyGroup
#include "bdc.h"

/* Returns the mask (bit i = voice i) of sounding voices (state 3 or 7) whose key group equals
   `group` (1..0x7f; 0 or out of range = none). Returns 0 while the voice layer is unusable. */

u32 SndSsVoiceMaskByKeyGroup(s32 group)
{
    u32 mask = 0;
    u32 bit;
    u32 i;
    SndSsVoice *voice;

    if (g_sndSsVoiceState != -1 && (u32)(group - 1) < 0x7f) {
        bit = 1;
        i = 0;
        mask = 0;
        if (g_sndSsVoiceCount != 0) {
            voice = g_sndSsVoices;
            do {
                i++;
                if ((voice->state == 3 || voice->state == 7) && voice->keyGroup == group) {
                    mask |= bit;
                }
                bit <<= 1;
                voice++;
            } while (i < g_sndSsVoiceCount);
        }
    }
    return mask;
}
