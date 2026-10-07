// bdc 0x089c8af0 SndBgmCmdStepPlay
#include "bdc.h"

/* Step function of command kind 3 (`SndBgmQueuePlay`), a three-stage state machine in `stage`:
   stage 0 waits its turn (advances at once when this is the only command of its channel,
   `SndBgmCountChannelCmds(channel) < 2`; otherwise only when the command is the first node of
   `g_sndBgmCmdList` (or the list is empty), so commands run in creation order); stage 1 creates
   the BGM player of `channel` if it is missing (and returns), then — unless the player already
   plays track `bgmId` (`SndBgmPlayerGetTrackId`), which advances at once — starts the track
   with `SndBgmPlayerPlayTrack``(player, bgmId, loop, flagB)` and advances once the player
   accepted it (retrying every frame until then); stage 2 and later, and negative stages, set
   `done`. */

void SndBgmCmdStepPlay(SndBgmCmd *cmd)
{
  s32 stage;
  s32 index;
  s32 count;
  s32 before;

  stage = cmd->stage;
  if (stage >= 1) {
    if (stage < 2) {
      if (!SndBgmPlayerExists(cmd->channel)) {
        SndBgmPlayerCreate(cmd->channel);
        return;
      }
      if (SndBgmPlayerGetTrackId(SndBgmPlayerGet(cmd->channel)) == cmd->bgmId) {
        cmd->stage = cmd->stage + 1;
        return;
      }
      if (SndBgmPlayerPlayTrack(SndBgmPlayerGet(cmd->channel), cmd->bgmId, cmd->loop,
                                cmd->flagB) == 0) {
        return;
      }
      cmd->stage = cmd->stage + 1;
      return;
    }
  }
  else if (stage >= 0) {
    if (SndBgmCountChannelCmds(cmd->channel) < 2) {
      cmd->stage = cmd->stage + 1;
      return;
    }
    index = 0;
    count = g_sndBgmCmdList->count;
    before = 0;
    if (index < count) {
      do {
        SndBgmCmd *node = SndBgmCmdListGetAt(g_sndBgmCmdList, index);
        index++;
        if (node == cmd) {
          break;
        }
        before++;
      } while (index < count);
    }
    if (before != 0) {
      return;
    }
    cmd->stage = cmd->stage + 1;
    return;
  }
  cmd->done = 1;
}
