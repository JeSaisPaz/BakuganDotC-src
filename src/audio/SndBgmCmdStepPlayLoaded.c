// bdc 0x089c8a78 SndBgmCmdStepPlayLoaded
#include "bdc.h"

/* Step function of command kind 2: starts the track that was already loaded on the channel. Stage 0
   creates the BGM player if it is missing (retrying next frame), otherwise calls
   `SndBgmPlayerPlay``(player, loop)`, which plays the file prepared by `SndBgmPlayerLoadTrack`,
   and advances to stage 1 when the player accepts; any later call marks the command done. No
   analysed code creates a kind-2 command, so the step is never reached. */

void SndBgmCmdStepPlayLoaded(SndBgmCmd *cmd)

{
  bool exists;
  SndBgmPlayer *player;
  s32 ok;
  
  if (cmd->stage == 0) {
    exists = SndBgmPlayerExists(cmd->channel);
    if (!exists) {
      SndBgmPlayerCreate(cmd->channel);
    }
    else {
      player = SndBgmPlayerGet(cmd->channel);
      ok = SndBgmPlayerPlay(player,cmd->loop);
      if (ok != 0) {
        cmd->stage = cmd->stage + 1;
      }
    }
  }
  else {
    cmd->done = '\x01';
  }
  return;
}

