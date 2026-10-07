// bdc 0x089fd530 IoDataMngReleaseOwner
#include "bdc.h"

/* Drops every reference `owner` holds on requests of the data manager (`CODataMng`,
   `g_ioDataMng`) (as `IoDataMngRelease` for each). */

void IoDataMngReleaseOwner(IoDataMng *self, void *owner)
{
  IoData *cur;
  IoData *next;

  CoreLockAcquire(self->lock);
  cur = (IoData *)self->requests->head;
  while (cur != (IoData *)0x0) {
    next = (IoData *)cur->base.next;
    if (IoDataHasOwner(cur, owner) != 0) {
      if (IoDataCountOwners(cur) == 1) {
        IoDataAddFlags(cur, 0x10);
      }
      else {
        IoDataRemoveOwner(cur, owner);
      }
    }
    cur = next;
  }
  CoreLockRelease(self->lock);
}
