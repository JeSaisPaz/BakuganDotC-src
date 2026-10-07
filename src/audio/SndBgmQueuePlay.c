// bdc 0x089c86f4 SndBgmQueuePlay
#include "bdc.h"

/* Queues a BGM play command: creates an `SndBgmCmd` task of kind 3 that starts BGM `bgmId` on BGM
   player `channel` (`SndBgmCmdStepPlay`). The last two arguments are forwarded to
   `SndBgmPlayerPlayTrack`. Called by `SndBgmPlayVoice`, `ScriptOpChangeBgm` and about 18
   other places. */

void SndBgmQueuePlay(s32 channel, s32 bgmId, u8 loop, u8 flagB)
{
    SndBgmCmd *cmd = (SndBgmCmd *)CoreTaskCreate(0x2756, 100);

    cmd->channel = channel;
    cmd->kind = 3;
    cmd->bgmId = bgmId;
    cmd->loop = loop;
    cmd->flagB = flagB;
}
