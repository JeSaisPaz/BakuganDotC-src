// bdc 0x088ed0f0 GameEventOp23MoveCamTargetY
#include "bdc.h"

/* Handler of event opcode 0x23 (`GameEventExecCommand`): adds `arg` along axis 1 to the target
   end key. */

void GameEventOp23MoveCamTargetY(GameEvent *self, u8 flag, s16 arg) {
    s32 delta[3];
    GameEventCamKeys *keys;

    delta[0] = 0;
    delta[1] = 0;
    delta[2] = 0;
    GameEventApplyAxisOffset(self, 1, (s16)flag, delta, 0);
    keys = self->camKeys;
    keys->targetEnd[0] += delta[0];
    keys->targetEnd[1] += delta[1];
    keys->targetEnd[2] += delta[2];
}
