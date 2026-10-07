// bdc 0x089fad7c IoDiscSimpleStateClose
#include "bdc.h"

/* State 6 handler of the `CODiscSimple` disc reader (`g_discSimple`) (vtable slot `+0x3c`):
   `sceIoCloseAsync(fd)`; on completion goes idle (state 0, fd -1). Goes to the suspended state 9
   when `suspendAfterClose` is set (also setting `abortClose`), when the power-suspend flag is set
   (except for mode-2 requests, which just clear it) or once the close has ended with `abortClose`
   set (error `0x80020329` excepted). */

void IoDiscSimpleStateClose(IoDiscSimple *self)
{
  int closeEnded;
  int ret;
  u8 suspend;

  closeEnded = 0;
  if (self->fd < 0) {
    self->asyncPending = 1;
  }
  if (!self->asyncPending) {
    ret = sceIoCloseAsync(self->fd);
    self->result = ret;
    suspend = self->powerSuspend;
    if (ret < 0) {
      closeEnded = 1;
    } else {
      self->asyncIssued = 1;
    }
  } else {
    ret = self->result;
    self->asyncPending = 0;
    suspend = self->powerSuspend;
    if (ret >= 0) {
      self->state = 0;
      self->idle = 1;
      self->fd = -1;
    }
    closeEnded = 1;
    if (self->suspendAfterClose) {
      self->state = 9;
      self->idle = 0;
      self->abortClose = 1;
    }
  }
  if (suspend) {
    if (self->mode != 2) {
      self->state = 9;
      self->idle = 0;
      self->result = 0;
      self->abortClose = 0;
      self->asyncPending = 0;
      return;
    }
    self->powerSuspend = 0;
  }
  if (closeEnded && self->abortClose && ret != (int)0x80020329) {
    self->state = 9;
    self->idle = 0;
  }
}
