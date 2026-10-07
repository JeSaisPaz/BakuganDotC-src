// bdc 0x088ea118 GameFieldGuardBlindStart
#include "bdc.h"

/* Starts the guard-blind period: save-profile word 0x2f = 180 and `GameFieldGuardBlindHideAll`.
   Called by `GameFieldPhaseMain`. */

void GameFieldGuardBlindStart(void *blind)

{
  SaveProfile *self;
  
  self = SaveGetProfile();
  SaveProfileSetWord(self,0x2f,0xb4);
  GameFieldGuardBlindHideAll(blind);
  return;
}

