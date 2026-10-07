// bdc 0x08940c44 UiScreen390ExitPhase
#include "bdc.h"

/* Phase 3 of `UiScreen390`: sets `closeRequested` (`+0x4c`), which closes the
   cutscene so that `ActorPlayerStateUseTerminal` can return the player to state 0. */

void UiScreen390ExitPhase(UiScreen *screen)

{
  screen->closeRequested = '\x01';
  return;
}

