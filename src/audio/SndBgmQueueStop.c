// bdc 0x089c875c SndBgmQueueStop
#include "bdc.h"

/* Queues a BGM stop command: creates an `SndBgmCmd` task of kind 4 for BGM player `channel`,
   which fades the channel out over `fadeSeconds` seconds (`SndBgmCmdStepStop`). Called from about
   40 places, among them the scene exit `GameFieldStopAllSound`, `SndBgmPlayVoice` and
   `ScriptOpChangeBgm`. */

void SndBgmQueueStop(float fadeSeconds, s32 channel)

{
  SndBgmCmd *cmd;

  cmd = (SndBgmCmd *)CoreTaskCreate(0x2756, 100);
  cmd->channel = channel;
  cmd->kind = 4;
  cmd->fadeSeconds = fadeSeconds;
  return;
}

