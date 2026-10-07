// bdc 0x088ec050 GameEventOp0BFade
#include "bdc.h"

/* Handler of event opcode 0x0b (`GameEventExecCommand`): queues a fade action of `value` frames
   (`GameEventAddFade`) and starts the active fader with `value - 1` (`GfxFaderStart`). */

void GameEventOp0BFade(GameEvent *self, u16 value, s16 arg)
{
  GfxFader *fader;

  GameEventAddFade(self, (u8)value, 0);
  fader = GfxGetActiveFader();
  GfxFaderStart(fader, value - 1);
}
