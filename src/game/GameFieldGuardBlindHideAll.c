// bdc 0x088ea0d4 GameFieldGuardBlindHideAll
#include "bdc.h"

/* Hides the guard view cones and the 0xbd9 gimmicks and puts the timer in state 0. */

void GameFieldGuardBlindHideAll(void *blind)

{
  GameFieldSetGuardConesVisible(blind,'\0');
  GameFieldSetGimmicks0bd9A(blind,'\0');
  GameFieldSetGimmicks0bd9B(blind,'\0');
  *(int *)blind = 0;
  return;
}

