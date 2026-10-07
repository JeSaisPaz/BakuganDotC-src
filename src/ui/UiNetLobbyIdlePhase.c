// bdc 0x08941dd0 UiNetLobbyIdlePhase
#include "bdc.h"

/* Empty phase handler (entry 0 of the phase table `0x08a9cf78`) of the ad-hoc network lobby screen
   (task 2000, `UiNetLobbyCtor`, created by the network menu task 1999); does nothing. */

void UiNetLobbyIdlePhase(UiScreen *screen)
{
    (void)screen;
}
