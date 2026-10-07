// bdc 0x089fd470 IoDataMngRelease
#include "bdc.h"

/* Drops `owner`'s reference to request `data` of the data manager (`CODataMng`, `g_ioDataMng`):
   when it is the last reference the request is flagged released (0x10, freed by
   `IoDataMngUpdate`), otherwise only the reference is removed. */

void IoDataMngRelease(IoDataMng *self, void *owner, void *data)
{
  IoData *cur;

  CoreLockAcquire(self->lock);
  cur = (IoData *)self->requests->head;
  while (cur != (IoData *)0x0) {
    if (cur == data) {
      if (IoDataHasOwner(cur, owner) != 0) {
        if (IoDataCountOwners(cur) == 1) {
          IoDataAddFlags(cur, 0x10);
        }
        else {
          IoDataRemoveOwner(cur, owner);
        }
      }
      break;
    }
    cur = (IoData *)cur->base.next;
  }
  CoreLockRelease(self->lock);
}
