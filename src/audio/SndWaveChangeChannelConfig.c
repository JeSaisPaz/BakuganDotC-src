// bdc 0x08a23db8 SndWaveChangeChannelConfig
#include "bdc.h"

/* Changes the format (stereo/mono) of output channel `channel` (0..3) with
   `sceAudioChangeChannelConfig`. Returns `0x80440010` for a bad channel. */

s32 SndWaveChangeChannelConfig(u32 channel, s32 format)
{
    s32 result = 0x80440010;

    if (channel < 4) {
        result = sceAudioChangeChannelConfig(g_sndWaveChannelIds[channel], format);
    }
    return result;
}
