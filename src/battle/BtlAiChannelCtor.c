// bdc 0x0888c334 BtlAiChannelCtor
#include "bdc.h"

/* Constructor of a 300-byte command channel of `BtlAi` (array-constructed ×4 at
   `ai+0x2d8` by `BtlAiCtor`, which passes it to `CxxVecNewSimple`): sets the current and pending
   command to 0x1b (idle), zeroes the phase and command timers and marks both expired, clears `rules`,
   constructs the array of ten 16-byte weight tables at `+0x50` (`CxxVecNewSimple` with element ctor
   `BtlAiChannelWeightEntryCtor`), zeroes the running command's timer (expired) and flags, clears the
   remaining fields with `BtlAiChannelClearState` and returns `self`. */

BtlAiChannel *BtlAiChannelCtor(BtlAiChannel *self)
{
    self->cmd = 0x1b;
    self->pendingCmd = 0x1b;
    self->phaseLimit = 0.0f;
    self->phaseElapsed = 0.0f;
    self->phaseExpired = 1;
    self->cmdLimit = 0.0f;
    self->cmdElapsed = 0.0f;
    self->cmdExpired = 1;
    self->rules = NULL;
    CxxVecNewSimple(self->weights, 10, sizeof(BtlAiWeightTable), BtlAiChannelWeightEntryCtor);
    self->exec.timerLimit = 0.0f;
    self->exec.timerElapsed = 0.0f;
    self->exec.timerExpired = 1;
    self->exec.flags = 0;
    BtlAiChannelClearState(self);
    return self;
}
