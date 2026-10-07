// bdc 0x08a2c734 GameEventActionStopNop
#include "bdc.h"

/* Entry 3 (stop, `+0x1c`) of the event action base vtable `0x08af6e20` (`GameEventActionDtor`):
   empty; overridden by `GameEventFadeActionStop`, `GameEventCamTweenStop`,
   `GameEventActorTweenStop` and `GameEventPropTweenStop`. */

void GameEventActionStopNop(GameEventAction *self, u8 apply)

{
  return;
}

