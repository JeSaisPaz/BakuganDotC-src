// bdc 0x088edf7c GameEventOp4ESetUiFlagA25
#include "bdc.h"

/* Handler of event opcode 0x4e (`GameEventExecCommand`): stores `flag` as the talk balloon portrait-enabled byte
   (0x08abea25) (`UiTalkBalloonSetPortraitEnabled`; cleared again when the event ends). */

void GameEventOp4ESetUiFlagA25(GameEvent *self, u8 flag, s16 arg)

{
  UiTalkBalloonSetPortraitEnabled(flag);
  return;
}

