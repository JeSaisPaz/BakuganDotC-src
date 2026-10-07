// bdc 0x089443b8 NetStatusTaskStateVisible
#include "bdc.h"

/* State 3 of the netplay status overlay: keeps animating the text (`NetStatusTaskAnimateText`)
   and switches to state 4 (`NetStatusTaskStateHide`) once the show flag `0x08ac1bd0` is cleared.
    */

void NetStatusTaskStateVisible(NetStatusTask *self)

{
  if (g_netStatusShow == '\0') {
    self->state = 4;
    self->step = 0;
  }
  NetStatusTaskAnimateText(self);
  return;
}

