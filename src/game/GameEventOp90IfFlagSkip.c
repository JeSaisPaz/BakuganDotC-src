// bdc 0x088f12d4 GameEventOp90IfFlagSkip
#include "bdc.h"

/* Handler of event opcode 0x90 (`GameEvent470ExecCommand`): when the selected flag is set, skips
   forward to the opcode-0x91 label whose argument equals `flag` (`GameEventSkipToOpcode`). */

void GameEventOp90IfFlagSkip(GameEvent470 *self, u8 flag, s16 arg) {
    if (GameEventFlagTest(self->testFlag)) {
        do {
            GameEventSkipToOpcode(&self->base, 0x91);
        } while (self->base.cmd->arg != flag);
    }
}
