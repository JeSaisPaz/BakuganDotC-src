// bdc 0x0894f0e0 UiNetMenuExitPhase
#include "bdc.h"

/* Exit phase (entry 3 of the phase table `0x08a9d360`) of the network-play menu (task 1999,
   `UiNetMenuCtor`; host-or-join choice that creates the lobby task 2000,
   `UiNetLobbyHostPhase`/`UiNetLobbyJoinPhase`): sets `closeRequested` (`+0x4c`). */

void UiNetMenuExitPhase(UiScreen *screen)

{
  screen->closeRequested = '\x01';
  return;
}

