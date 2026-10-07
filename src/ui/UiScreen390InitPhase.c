// bdc 0x089405b4 UiScreen390InitPhase
#include "bdc.h"

/* Phase 0 of `UiScreen390` (phase table `0x08a9ced0`): waits one frame, then
   advances to `UiScreen390FadeInPhase` with the frame timer `+0x90` set to 40. */

void UiScreen390InitPhase(UiScreen *screen)

{
  if (screen->phaseStep == 0) {
    screen->phaseStep = 1;
    return;
  }
  screen->phase = screen->phase + 1;
  ((UiScreen390 *)screen)->frameTimer = 0x28;
  screen->phaseStep = 0;
  return;
}

