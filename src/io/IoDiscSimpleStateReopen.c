// bdc 0x089fa540 IoDiscSimpleStateReopen
#include "bdc.h"

/* State 10 handler of the `CODiscSimple` disc reader (vtable slot `+0x54`, dispatched by
   `IoDiscSimpleUpdate`): restarts the request at the open state (state word `+0x0` = 1) and
   clears the byte `+0xa`. */

void IoDiscSimpleStateReopen(IoDiscSimple *self)

{
  self->state = 1;
  self->asyncPending = '\0';
  return;
}

