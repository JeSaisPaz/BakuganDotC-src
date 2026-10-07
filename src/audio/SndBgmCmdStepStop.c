// bdc 0x089c8c50 SndBgmCmdStepStop
#include "bdc.h"

/* Step function of command kind 4 (`SndBgmQueueStop`, 'fade the BGM out and stop'), a four-stage
   state machine in `stage`: stage 0 cancels the older queued commands of the same channel that
   precede this one in `g_sndBgmCmdList` (sets their `done`) and waits until they are gone; stage
   1 creates the BGM player if it is missing, otherwise starts the stop with the fade time
   `fadeSeconds` (`SndBgmPlayerStopF`, seconds are converted to milliseconds there) and advances
   once it is accepted; stage 2 advances once the player is missing or reports it stopped
   (`SndBgmPlayerIsStopped`); stage 3 and later (or negative) set `done`. */

void SndBgmCmdStepStop(SndBgmCmd *cmd)
{
  SndBgmCmd *other;
  s32 count;
  s32 cancelled;
  s32 i;

  switch (cmd->stage) {
  case 0:
    if (SndBgmCountChannelCmds(cmd->channel) >= 2) {
      count = g_sndBgmCmdList->count;
      cancelled = 0;
      for (i = 0; i < count; i++) {
        other = SndBgmCmdListGetAt(g_sndBgmCmdList, i);
        if (other == cmd) {
          break;
        }
        if (other->channel == cmd->channel) {
          other->done = 1;
          cancelled++;
        }
      }
      if (cancelled > 0) {
        return;
      }
    }
    cmd->stage++;
    /* fall through */
  case 1:
    if (SndBgmPlayerExists(cmd->channel)) {
      if (SndBgmPlayerStopF(cmd->fadeSeconds, SndBgmPlayerGet(cmd->channel), 0) != 0) {
        cmd->stage++;
      }
    } else {
      SndBgmPlayerCreate(cmd->channel);
    }
    return;
  case 2:
    if (SndBgmPlayerExists(cmd->channel)) {
      if (SndBgmPlayerIsStopped(SndBgmPlayerGet(cmd->channel))) {
        cmd->stage++;
      }
    } else {
      cmd->stage++;
    }
    return;
  default:
    cmd->done = 1;
    return;
  }
}
