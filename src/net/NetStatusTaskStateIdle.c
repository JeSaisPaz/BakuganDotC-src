// bdc 0x08944030 NetStatusTaskStateIdle
#include "bdc.h"

/* State 1 of the netplay status overlay: waits until
   `NetStatusSetMessage` sets the show flag `0x08ac1bd0`, then goes to state 2
   (`NetStatusTaskStateShow`) with sub-step and timer cleared. */

void NetStatusTaskStateIdle(NetStatusTask *self)

{
  if (g_netStatusShow != '\0') {
    self->state = 2;
    self->step = 0;
    self->frame = 0;
  }
  return;
}

