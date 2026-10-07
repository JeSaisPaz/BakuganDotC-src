// bdc 0x089c8a04 SndBgmCmdStepPreload
#include "bdc.h"

/* Step function of command kind 1 (`SndBgmQueuePreload`). Stage 0: if the BGM player of `channel`
   does not exist yet it creates it (`SndBgmPlayerCreate`) and retries next frame, otherwise it
   asks the stream file loader to preload the file of BGM `bgmId` (`SndStreamFilePreload`) and
   advances to stage 1. Any later call marks the command done. */

void SndBgmCmdStepPreload(SndBgmCmd *cmd)

{
  bool exists;
  s32 id;
  
  if (cmd->stage == 0) {
    exists = SndBgmPlayerExists(cmd->channel);
    if (!exists) {
      SndBgmPlayerCreate(cmd->channel);
    }
    else {
      id = cmd->bgmId;
      SndBgmPlayerGet(cmd->channel);
      SndStreamFilePreload(id);
      cmd->stage = cmd->stage + 1;
    }
  }
  else {
    cmd->done = '\x01';
  }
  return;
}

