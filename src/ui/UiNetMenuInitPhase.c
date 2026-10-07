// bdc 0x0894f05c UiNetMenuInitPhase
#include "bdc.h"

/* Init phase (entry 0 of the phase table `0x08a9d360`) of the network-play menu (task 1999,
   `UiNetMenuCtor`; host-or-join choice that creates the lobby task 2000,
   `UiNetLobbyHostPhase`/`UiNetLobbyJoinPhase`): advances to phase 1 immediately (clears
   `phaseStep`). */

void UiNetMenuInitPhase(UiScreen *screen)

{
  if (screen->phaseStep == 0) {
    screen->phaseStep = 1;
  }
  screen->phaseStep = 0;
  screen->phase = screen->phase + 1;
  return;
}

