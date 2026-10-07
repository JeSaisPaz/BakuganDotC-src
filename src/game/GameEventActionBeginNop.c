// bdc 0x08a2c72c GameEventActionBeginNop
#include "bdc.h"

/* Entry 2 (start, `+0x14`) of the event action base vtable `0x08af6e20` (`GameEventActionDtor`):
   empty; inherited by the actor and prop tweens, overridden by `GameEventFadeActionBegin` and
   `GameEventCamTweenBegin`. */

void GameEventActionBeginNop(GameEventAction *self)

{
  return;
}

