// bdc 0x088edfc4 GameEventOp51SetUiFlagA26
#include "bdc.h"

/* Handler of event opcode 0x51 (`GameEventExecCommand`): stores `flag` as the talk balloon frame style byte
   (0x08abea26) (`UiTalkBalloonSetFrameStyle`). */

void GameEventOp51SetUiFlagA26(GameEvent *self, u8 flag, s16 arg)

{
  UiTalkBalloonSetFrameStyle(flag);
  return;
}

