// bdc 0x08892dec BtlAiTargetInRecoveryState
#include "bdc.h"

/* Returns 1 when `unit` is non-NULL and the current target of `BtlAi` has flag 0x100
   in `stateFlags` and is in state 8 or 0xb. Threat test of `BtlAiRunReactionRules` /
   `BtlAiGuardLayerRun`. */
s32 BtlAiTargetInRecoveryState(BtlAi *self, void *unit)
{
  BtlBakugan *target;

  if (unit != NULL) {
    target = self->target;
    if (target != NULL && (target->stateFlags & 0x100) != 0 &&
        (target->state == 8 || target->state == 0xb)) {
      return 1;
    }
  }
  return 0;
}
