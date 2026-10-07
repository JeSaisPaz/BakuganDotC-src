// bdc 0x089fd5e8 IoDataMngUpdate
#include "bdc.h"

/* One step of the data thread (`BootDataThread`) over the data manager (`CODataMng`,
   `g_ioDataMng`): under its lock deletes released requests (flag 0x10 without 0x80000000) and
   runs `IoDataExec` on the others; records whether everything is idle (`+0x10`), then sleeps 100
   us while work remains or puts the thread to sleep. */

void IoDataMngUpdate(IoDataMng *self)
{
    bool busy;
    bool release;
    IoData *req;
    IoData *next;

    CoreLockAcquire(self->lock);
    busy = false;
    req = (IoData *)self->requests->head;
    while (req != NULL) {
        release = false;
        if (IoDataHasFlags(req, 0x10) && !IoDataHasFlags(req, 0x80000000)) {
            release = true;
        }
        if (release) {
            next = (IoData *)req->base.next;
            IoDataRemoveOwner(req, NULL);
            if (self->pool != NULL && MemPoolFree(self->pool, req)) {
                /* Pool item: destroy without freeing (flag 2). */
                const VtblEntry *dtor = &((const VtblEntry *)req->base.vtable)[1];

                ((void (*)(void *, s32))dtor->fn)((u8 *)req + dtor->delta, 2);
                req = NULL;
            }
            if (req != NULL) {
                /* Heap item: deleting destructor (flag 3). */
                const VtblEntry *dtor = &((const VtblEntry *)req->base.vtable)[1];

                ((void (*)(void *, s32))dtor->fn)((u8 *)req + dtor->delta, 3);
            }
            busy = true;
            req = next;
        } else {
            if (IoDataExec(req) != 0) {
                busy = true;
            }
            req = (IoData *)req->base.next;
        }
    }
    self->idle = !busy;
    CoreLockRelease(self->lock);
    if (busy) {
        sceKernelDelayThreadCB(100);
    } else {
        BootSleepCurrentThread();
    }
}
