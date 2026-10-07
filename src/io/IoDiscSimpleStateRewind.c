// bdc 0x089fa47c IoDiscSimpleStateRewind
#include "bdc.h"

/* State 7 handler of the `CODiscSimple` disc reader (`g_discSimple`) (vtable slot `+0x44`):
   `sceIoLseekAsync(fd, 0, SEEK_SET)` and, on success, state 6 (close). */

void IoDiscSimpleStateRewind(IoDiscSimple *self)

{
  int ret;
  
  if (self->asyncPending == '\0') {
    ret = sceIoLseekAsync(self->fd,g_zeroFileOffsetB
                            ,0);
    self->result = ret;
    if (-1 < ret) {
      self->asyncIssued = '\x01';
    }
  }
  else {
    self->asyncPending = '\0';
    if (-1 < self->result) {
      self->state = 6;
    }
  }
  return;
}

