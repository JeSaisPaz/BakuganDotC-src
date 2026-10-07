// bdc 0x089fa288 IoDiscSimpleStateOpen
#include "bdc.h"

/* State 1 handler of the `CODiscSimple` disc reader (`g_discSimple`) (vtable slot `+0x1c`): first
   pass builds the full path (`IoMakePath`) and starts `sceIoOpenAsync(path, PSP_O_RDONLY)`; on
   completion stores the fd (`+0x14`) and moves to state 2 (read: get size), 7 (mode 1: rewind) or 8
   (mode 2: get sector), setting the open-result flags. A failed open goes idle when error flags
   (`+0x48`) or abort (`+0xf4`) ask for it, or when the path is empty; otherwise the open is
   retried. */

void IoDiscSimpleStateOpen(IoDiscSimple *self)
{
    char path[256];
    SceUID fd;
    int emptyPath;

    self->abortClose = 0;
    self->suspendAfterClose = 0;
    self->dataLoaded = 0;
    if (!self->asyncPending) {
        memset(path, 0, sizeof(path));
        self->powerSuspend = 0;
        IoMakePath(self->path, path);
        fd = sceIoOpenAsync(path, 1, 0);
        self->result = fd;
        if (fd >= 0) {
            self->asyncIssued = 1;
            self->fd = fd;
            self->result = 0;
        }
        return;
    }

    self->asyncPending = 0;
    if (self->result >= 0) {
        switch (self->mode) {
        case 1:
            self->state = 7;
            break;
        case 2:
            self->state = 8;
            break;
        default:
            self->state = 2;
            break;
        }
        self->openDecided = 1;
        self->openOk = 1;
        return;
    }

    if (self->errorFlags) {
        self->openDecided = 1;
        self->openOk = 0;
        self->state = 0;
        self->idle = 1;
        return;
    }
    if (self->abortFlag) {
        self->state = 0;
        self->idle = 1;
        return;
    }
    emptyPath = 0;
    if (self->path == NULL) {
        emptyPath = 1;
    } else if (strlen(self->path) == 0) {
        emptyPath = 1;
    }
    if (emptyPath) {
        self->state = 0;
        self->idle = 1;
    }
}
