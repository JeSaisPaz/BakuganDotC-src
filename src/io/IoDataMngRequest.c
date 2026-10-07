// bdc 0x089fd120 IoDataMngRequest
#include "bdc.h"

/* Requests file `path` from the data manager (`CODataMng`, `g_ioDataMng`) for `owner`: unless
   `forceNew`, reuses an existing request for the same path (adds `owner`, clears its release flag
   0x10); otherwise creates a new request (`IoDataCtor`, from the pool or the heap, low end when
   `buffer & 1`), stores the manager lock, appends it to the active list, sets the path (copied when
   `copyPath`), adds `owner` and sets the destination `buffer` (`IoDataSetBuffer`; bit 0 also
   selects the low heap end). Returns the request. */

void *IoDataMngRequest(IoDataMng *self, void *owner, char *path, u32 buffer, bool forceNew, bool copyPath)
{
    bool fromLow;
    bool found;
    bool wasLow;
    IoData *req;
    IoData *mem;

    fromLow = (buffer & 1) != 0;
    CoreLockAcquire(self->lock);
    found = false;
    req = (IoData *)self->requests->head;
    if (!forceNew) {
        for (; req != NULL; req = (IoData *)req->base.next) {
            if (strcmp(IoDataGetPath(req), path) == 0) {
                found = true;
                IoDataAddOwner(req, owner, fromLow);
                if (IoDataHasFlags(req, 0x10)) {
                    IoDataClearFlags(req, 0x10);
                }
                break;
            }
        }
    }
    if (!found) {
        req = NULL;
        if (self->pool != NULL) {
            req = MemPoolAlloc(self->pool);
        }
        if (req != NULL) {
            IoDataCtor(req);
        } else {
            MemLock();
            wasLow = MemIsAllocFromLow();
            MemSetAllocFromLow(fromLow);
            mem = MemAlloc(0x60, NULL, 0);
            MemSetAllocFromLow(wasLow);
            MemUnlock();
            if (mem != NULL) {
                IoDataCtor(mem);
            }
            req = mem;
        }
        /* No NULL check: a failed heap allocation faults here, as in the original. */
        req->lock = self->lock;
        CoreNodeOwnerAppend(self->requests, &req->base);
        if (copyPath) {
            IoDataSetPathCopy(req, path, fromLow);
        } else {
            IoDataSetPath(req, path);
        }
        IoDataAddOwner(req, owner, fromLow);
        IoDataSetBuffer(req, (void *)(uintptr_t)buffer);
    }
    CoreLockRelease(self->lock);
    return req;
}
