// bdc 0x089c6ef4 SndManagerStateInitAudio
#include "bdc.h"

/* State 3 of the sound manager (`SndManagerStep`): brings up the audio hardware path. It bumps a
   global init counter (`g_sndAudioInitCount`), reserves the PSP audio output (`sceAudioOutput2Reserve`
   with 0x100 samples), initialises the Sony sound-system layer (`SndSsInit(0x14, 0x100)`,
   `SndWaveInit()`, then `SndWaveChangeChannelConfig(i, 0)` and `SndWaveSetChannelDataLen(i, 0x100)`
   for channels 0..2), moves the manager to state 4 and registers the power callbacks
   `SndManagerOnSuspend` and `SndManagerOnResume` with `CorePowerAddSuspendCallback` /
   `CorePowerAddResumeCallback`. */

void SndManagerStateInitAudio(SndManager *mgr)
{
    g_sndAudioInitCount = g_sndAudioInitCount + 1;
    sceAudioOutput2Reserve(0x100);
    SndSsInit(0x14, 0x100);
    SndWaveInit();
    SndWaveChangeChannelConfig(0, 0);
    SndWaveChangeChannelConfig(1, 0);
    SndWaveChangeChannelConfig(2, 0);
    SndWaveSetChannelDataLen(0, 0x100);
    SndWaveSetChannelDataLen(1, 0x100);
    SndWaveSetChannelDataLen(2, 0x100);
    mgr->state = 4;
    CorePowerAddSuspendCallback(CorePowerGet(), SndManagerOnSuspend);
    CorePowerAddResumeCallback(CorePowerGet(), SndManagerOnResume);
}
