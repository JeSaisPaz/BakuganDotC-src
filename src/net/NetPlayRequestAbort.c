// bdc 0x0881bb98 NetPlayRequestAbort
#include "bdc.h"

/* Sets the abort-requested byte (`+0xe`) of the NetPlay object. On its next run `NetPlayUpdate`
   forces the state machine into state 8 (see `NetPlayStateAbort`), restarting the substate at 0,
   unless it is already in state 8. */

void NetPlayRequestAbort(NetPlay *self)

{
  self->abortRequested = '\x01';
  return;
}

