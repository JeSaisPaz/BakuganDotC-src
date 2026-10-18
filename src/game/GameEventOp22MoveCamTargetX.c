// bdc 0x088ed074 GameEventOp22MoveCamTargetX
#include "bdc.h"

/* Handler of event opcode 0x22 (`GameEventExecCommand`): adds `arg` along axis 0 to the target
   end key. */

void GameEventOp22MoveCamTargetX(GameEvent *self, s16 arg, s16 unused) {
    s32 delta[3];
    GameEventCamKeys *keys;

    delta[0] = 0;
    delta[1] = 0;
    delta[2] = 0;
    GameEventApplyAxisOffset(self, 0, arg, delta, 0);
    keys = self->camKeys;
    keys->targetEnd[0] += delta[0];
    keys->targetEnd[1] += delta[1];
    keys->targetEnd[2] += delta[2];
}
