// bdc 0x08a23df4 SndWaveGetChannelRestLength
#include "bdc.h"

/* Returns the number of samples still queued on output channel `channel`
   (`sceAudioGetChannelRestLength`), or `0x80440010` for a bad channel. */

s32 SndWaveGetChannelRestLength(u32 channel)
{
    s32 result = 0x80440010;

    if (channel < 4) {
        result = sceAudioGetChannelRestLength(g_sndWaveChannelIds[channel]);
    }
    return result;
}
