// bdc 0x088e950c ActorNpcGuardIsSpecialRoute
#include "bdc.h"

/* Vtable slot 19 of the guard classes: true on stage 0xc (script global 1) for the guard whose
   placement route index (`record+0x3b`) is 0xa3. */

s32 ActorNpcGuardIsSpecialRoute(ActorNpc *self)
{
  ActorNpcPlacement *placement;

  if (g_scriptGlobalVars[1] == 0xc) {
    placement = (ActorNpcPlacement *)self->base.placement;
    if (placement != NULL && placement->routeIndex == 0xa3) {
      return 1;
    }
  }
  return 0;
}
