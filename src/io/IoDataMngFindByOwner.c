// bdc 0x089fd3c4 IoDataMngFindByOwner
#include "bdc.h"

/* Returns the next live request of the data manager (`CODataMng`, `g_ioDataMng`) after `prev`
   (from the start when NULL) that `owner` references, or NULL. */

void *IoDataMngFindByOwner(IoDataMng *self, void *owner, void *prev)
{
    IoData *cur;
    IoData *found = NULL;

    CoreLockAcquire(self->lock);
    if (prev == NULL) {
        cur = (IoData *)self->requests->head;
    } else {
        cur = (IoData *)((IoData *)prev)->base.next;
    }
    while (cur != NULL) {
        if (IoDataHasOwner(cur, owner) && !IoDataHasFlags(cur, 0x10)) {
            found = cur;
            break;
        }
        cur = (IoData *)cur->base.next;
    }
    CoreLockRelease(self->lock);
    return found;
}
