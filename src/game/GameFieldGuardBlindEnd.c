// bdc 0x088ea1f8 GameFieldGuardBlindEnd
#include "bdc.h"

/* Ends the guard-blind period: shows the view cones and 0xbd9 gimmicks again, clears save-profile
   word 0x2f and puts the timer in state 2. */

void GameFieldGuardBlindEnd(void *blind)

{
  SaveProfile *self;
  
  GameFieldSetGuardConesVisible(blind,'\x01');
  GameFieldSetGimmicks0bd9A(blind,'\x01');
  GameFieldSetGimmicks0bd9B(blind,'\x01');
  self = SaveGetProfile();
  SaveProfileSetWord(self,0x2f,0);
  *(int *)blind = 2;
  return;
}

