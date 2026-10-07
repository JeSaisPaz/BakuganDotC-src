// bdc 0x089fd034 IoDataMngDtor
#include "bdc.h"

/* Destructor of the data manager (`CODataMng`, `g_ioDataMng`): destroys the request pool, deletes
   both owner lists (virtual deleting destructor, vtable entry 1, flag 3), destroys the lock and
   frees the object when `flags & 1`. A NULL `self` does nothing. */

void IoDataMngDtor(IoDataMng *self, u32 flags)
{
    const VtblEntry *dtor;

    if (self == NULL) {
        return;
    }
    if (self->pool != NULL) {
        MemPoolDestroy(self->pool, 3);
        self->pool = NULL;
    }
    if (self->requests != NULL) {
        dtor = &((const VtblEntry *)self->requests->vtable)[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)self->requests + dtor->delta, 3);
        self->requests = NULL;
    }
    if (self->ownerRefs != NULL) {
        dtor = &((const VtblEntry *)self->ownerRefs->vtable)[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)self->ownerRefs + dtor->delta, 3);
        self->ownerRefs = NULL;
    }
    if (self->lock != NULL) {
        CoreLockDestroy(self->lock, 3);
        self->lock = NULL;
    }
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
