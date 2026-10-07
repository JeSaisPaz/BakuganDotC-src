// bdc 0x0881b22c NetPlaySetState
#include "bdc.h"

/* Switches the `NetPlay` state machine to `state` (`+4`) and resets the substate `+8`
   to 0. */

void NetPlaySetState(NetPlay *self, s32 state)

{
  self->state = state;
  self->substate = 0;
  return;
}

