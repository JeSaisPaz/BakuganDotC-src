// bdc 0x089c86ac SndBgmQueuePreload
#include "bdc.h"

/* Queues a BGM preload command: creates an `SndBgmCmd` task of kind 1 that asks BGM player
   `channel` to pre-read the stream file of BGM `bgmId` into memory (`SndBgmCmdStepPreload`, which
   ends in `SndStreamFilePreload`). Sibling of `SndBgmQueuePlay` (kind 3) and
   `SndBgmQueueStop` (kind 4); called twice by `BtlMainPhaseLoad` (`0x0885281c`, `0x08852838`)
   to pre-read the battle BGM. */

void SndBgmQueuePreload(s32 channel, s32 bgmId)

{
  SndBgmCmd *cmd;

  cmd = (SndBgmCmd *)CoreTaskCreate(0x2756, 100);
  cmd->channel = channel;
  cmd->kind = 1;
  cmd->bgmId = bgmId;
  return;
}

