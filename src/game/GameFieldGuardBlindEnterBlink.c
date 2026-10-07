// bdc 0x088ea154 GameFieldGuardBlindEnterBlink
#include "bdc.h"

/* Switches the guard-blind timer to its blink state 1 (counter `+4` and toggle `+8` cleared). */

void GameFieldGuardBlindEnterBlink(GameFieldGuardBlind *blind)
{
  blind->counter = 0;
  blind->toggle = 0;
  blind->state = 1;
}
