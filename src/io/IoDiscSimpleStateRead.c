// bdc 0x089fa3dc IoDiscSimpleStateRead
#include "bdc.h"

/* State 4 handler of the `CODiscSimple` disc reader (`g_discSimple`) (vtable slot `+0x2c`): reads
   the whole file into the destination with `sceIoReadAsync(fd, dest, size)`; on completion writes
   back/invalidates the data cache and moves to state 6 (close). Error `0x80010005` (interrupted)
   also goes to close with the retry flag `+0xec` set. */

void IoDiscSimpleStateRead(IoDiscSimple *self)

{
  int ret;
  
  if (self->asyncPending == '\0') {
    ret = sceIoReadAsync(self->fd,self->dest,self->size);
    self->result = ret;
    if (ret == 0) {
      self->asyncIssued = '\x01';
    }
  }
  else {
    self->asyncPending = '\0';
    if (self->result < 0) {
      if (self->result == -0x7ffefffb) {
        self->openOk = '\0';
        self->dataLoaded = '\0';
        self->abortClose = '\x01';
        self->state = 6;
      }
    }
    else {
      sceKernelDcacheWritebackInvalidateRange(self->dest,self->size);
      self->dataLoaded = '\x01';
      self->state = 6;
    }
  }
  return;
}

