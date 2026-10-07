// bdc 0x089d2f80 NetPdpRequestClose
#include "bdc.h"

/* Moves a running `CONetPDP` (state 2) to the closing state 3 and returns 1; returns 0 in any other
   state. */

int NetPdpRequestClose(NetPdp *self)

{
  if (self->state == 2) {
    self->state = 3;
    return 1;
  }
  return 0;
}
