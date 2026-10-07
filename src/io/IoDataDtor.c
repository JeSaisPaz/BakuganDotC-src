// bdc 0x089fb78c IoDataDtor
#include "bdc.h"

/* Destructor of a data request (`COData`) (vtable `g_ioDataVtbl` slot 1): frees the data buffer
   `buffer` when the request owns it (`ownsBuffer`, buffer not 0/1) and the owned path copy
   `pathCopy`, runs `CoreNodeDtor` and frees the node when `flags & 1`. A NULL `self` does
   nothing. */

void IoDataDtor(IoData *self, u32 flags)
{
    if (self == NULL) {
        return;
    }
    self->base.vtable = g_ioDataVtbl;
    if (self->ownsBuffer && self->buffer != NULL && (uintptr_t)self->buffer != 1) {
        MemLock();
        MemFree(self->buffer, NULL, 0);
        MemUnlock();
        self->buffer = NULL;
    }
    if (self->pathCopy != NULL) {
        MemLock();
        MemFree(self->pathCopy, NULL, 0);
        MemUnlock();
        self->pathCopy = NULL;
    }
    CoreNodeDtor(&self->base, 0);
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
