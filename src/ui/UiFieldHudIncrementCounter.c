// bdc 0x088cf050 UiFieldHudIncrementCounter
#include "bdc.h"

/* Increments the counter `+0xd0` of the field HUD (task 3001, `UiFieldHudCtor`; sprite array
   `+0x1c`, player `+0x74`) (targets counted by `UiFieldHudCountTargets`). Callers:
   `GameGimmickCollidableUpdate` and `ActorNpcSwitchRobotStateShutdown`. */

void UiFieldHudIncrementCounter(UiFieldHud *self)

{
  self->targetsDone = self->targetsDone + 1;
  return;
}

