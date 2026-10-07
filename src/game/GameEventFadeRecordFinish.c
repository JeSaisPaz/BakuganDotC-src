// bdc 0x088f2970 GameEventFadeRecordFinish
#include "bdc.h"

/* Jumps a fade record to its end state: bytes 0 and 2 = byte 1. */

void GameEventFadeRecordFinish(GameEventFadeRecord *self)

{
  self->start = self->target;
  self->cur = self->target;
  return;
}

