// bdc 0x088cf018 UiFieldHudDisable
#include "bdc.h"

/* Disables the field HUD (`g_uiFieldHudEnabled = 0`, see `UiFieldHudIsSuspended`) and resets the
   player's HUD-related state (`ActorPlayerCancelPowers`). Caller: `GameFieldPhaseMain`. */

void UiFieldHudDisable(void)

{
  ActorPlayer *self;
  
  self = ActorFindPlayer();
  if (self != (ActorPlayer *)0x0) {
    ActorPlayerCancelPowers(self);
  }
  g_uiFieldHudEnabled = 0;
  return;
}

