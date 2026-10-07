// bdc 0x08a32560 IoDiscSimpleSlot2Nop
#include "bdc.h"

/* Empty virtual slot 2 (`+0x14`) of the `CODiscSimple` disc reader vtable `0x08af5894` (between
   `IoDiscSimpleUpdate` and the state handlers starting with `IoDiscSimpleStateOpen`). */

void IoDiscSimpleSlot2Nop(IoDiscSimple *self)

{
  return;
}

