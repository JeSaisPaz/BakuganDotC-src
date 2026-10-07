// bdc 0x089406e8 UiScreen390FadeInPhase
#include "bdc.h"

/* Phase 1 of `UiScreen390` (phase table `0x08a9ced0`): step 0 starts a 30-frame
   fade from black and advances the step; any other step advances the phase, picks the terminal
   (`UiScreen390SelectTerminal`), computes the angle towards it (`UiScreen390ComputeTerminalAngle`)
   and clears its current effect (`UiScreen390ClearTerminalEffect`). */

void UiScreen390FadeInPhase(UiScreen *screen)
{
  GfxFader *fader;

  if (screen->phaseStep == 0) {
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 1.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 0.0f;
    fader = GfxGetActiveFader();
    GfxFaderStart(fader, 30);
    screen->phaseStep = screen->phaseStep + 1;
  }
  else {
    screen->phase = screen->phase + 1;
    UiScreen390SelectTerminal(screen);
    UiScreen390ComputeTerminalAngle(screen);
    UiScreen390ClearTerminalEffect(screen);
  }
}
