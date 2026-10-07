// bdc 0x088f2980 GameEventFadeActionStop
#include "bdc.h"

/* Stop method (slot 3) of the screen-fade action (vtable `0x08af42d4`, 0xc bytes): finishes the
   record (`GameEventFadeRecordFinish`). */

void GameEventFadeActionStop(GameEventFadeAction *self, u8 apply)

{
  GameEventFadeRecordFinish(self->rec);
  return;
}

