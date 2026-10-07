// bdc 0x088edf98 GameEventOp4FSetUiFlagA27
#include "bdc.h"

/* Handler of event opcode 0x4f (`GameEventExecCommand`): stores `flag` in the talk balloon text colour and
   `arg` in the text colour parameter (`UiTalkBalloonSetTextColor`). */

void GameEventOp4FSetUiFlagA27(GameEvent *self, u8 flag, s16 arg)

{
  UiTalkBalloonSetTextColor(flag,arg);
  return;
}

