// bdc 0x0881b26c NetPlaySelectHost
#include "bdc.h"

/* Copies peer record `i` (0x1f bytes) into the selected-host record `+0x94` of the
   `NetPlay` manager and sets the 'host selected' byte `+0xb3`; an invalid index leaves
   the record zeroed. Used by `UiNetLobbyJoinPhase` when the player picks a game to join. */

void NetPlaySelectHost(NetPlay *self, s32 i)
{
  memset(&self->selectedHost, 0, 0x1f);
  if ((-1 < i) && (i < self->peerCount)) {
    self->selectedHost = self->peers[i];
    self->hasSelectedHost = 1;
  }
}
