// bdc 0x088e9f30 GameFieldGuardBlindCtor
#include "bdc.h"

/* Constructor of the field's guard-blind timer (state `+0` = 2 idle, toggle `+8` = 0). Called by
   `GameFieldCtor`. */

GameFieldGuardBlind *GameFieldGuardBlindCtor(GameFieldGuardBlind *blind)
{
  blind->state = 2;
  blind->toggle = 0;
  return blind;
}
