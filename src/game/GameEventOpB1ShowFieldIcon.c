// bdc 0x088f1a1c GameEventOpB1ShowFieldIcon
#include "bdc.h"

/* Handler of event opcode 0xb1 (`GameEvent470ExecCommand`): with `arg` 0: waits on `+0x40` (wait
   type 4) and clears the HUD flags g_uiTalkBalloonPortraitEnabled and g_uiTalkBalloonFrameStyle; otherwise shows field icon `arg`
   (`GameFieldSetLocationSprite`, sprites `+0x6b0..+0x6b8` of the field task). */

void GameEventOpB1ShowFieldIcon(GameEvent *self, u8 flag, s16 arg)

{
  CoreTask *task;
  
  if ((arg & 0xffU) == 0) {
    self->msgRequest = 1;
    self->waitType = '\x04';
    UiTalkBalloonSetFrameStyle('\0');
    UiTalkBalloonSetPortraitEnabled('\0');
  }
  else {
    task = CoreTaskFind(500);
    GameFieldSetLocationSprite(task,(u8)arg);
  }
  return;
}

