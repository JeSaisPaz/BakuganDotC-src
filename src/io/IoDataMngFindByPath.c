// bdc 0x089fd320 IoDataMngFindByPath
#include "bdc.h"

/* Returns the first live (not released, flag 0x10 clear) request of the data manager (`CODataMng`,
   `g_ioDataMng`) for `path`, or NULL. */

void *IoDataMngFindByPath(IoDataMng *self, char *path)
{
    IoData *cur;
    IoData *found = NULL;

    CoreLockAcquire(self->lock);
    cur = (IoData *)self->requests->head;
    while (cur != NULL) {
        if (strcmp(IoDataGetPath(cur), path) == 0 && !IoDataHasFlags(cur, 0x10)) {
            found = cur;
            break;
        }
        cur = (IoData *)cur->base.next;
    }
    CoreLockRelease(self->lock);
    return found;
}
