// bdc 0x089fabcc IoDiscSimpleStateStreamRead
#include "bdc.h"

/* State 5 handler of the `CODiscSimple` disc reader (`g_discSimple`) (vtable slot `+0x34`): reads
   the next chunk of a streamed file into the current buffer node (`sceIoReadAsync(fd, node->data,
   0x30000)`); on completion marks the node filled with its length, adds to the total, and either
   closes (short or empty read, state 6) or claims the next free node and continues. A read error
   of `0x80010005` aborts into the close state. Returns 1 when a new node was claimed, else 0. */

int IoDiscSimpleStateStreamRead(IoDiscSimple *self)
{
    IoDiscBufNode *node;
    int started;
    int r;
    int n;

    started = 0;
    if (!self->asyncPending) {
        r = sceIoReadAsync(self->fd, self->curNode->data, 0x30000);
        self->result = r;
        if (r == 0) {
            self->asyncIssued = 1;
        }
        return started;
    }

    r = self->result;
    if (r < 0) {
        self->asyncPending = 0;
        if (r == (int)0x80010005) {
            self->openOk = 0;
            self->dataLoaded = 0;
            self->abortClose = 1;
            self->state = 6;
        }
        return started;
    }

    if (r == 0) {
        self->curNode->state = 0;
        self->curNode = NULL;
        self->state = 6;
        self->asyncPending = 0;
    } else if (r <= 0x30000) {
        node = self->curNode;
        if (node != NULL && node->state == 1) {
            sceKernelDcacheWritebackInvalidateRange(node->data, r);
            n = self->result;
            self->curNode->len = n;
            self->streamBytes += n;
            self->curNode->state = 2;
            if (self->curNode->len < 0x30000) {
                self->state = 6;
                self->asyncPending = 0;
            }
            self->curNode = NULL;
        }
        if (self->state != 6 && self->curNode == NULL) {
            node = self->bufNodes[self->writeIndex];
            if (node->state == 0) {
                self->curNode = node;
                node->state = 1;
                if (++self->writeIndex >= 2) {
                    self->writeIndex = 0;
                }
                started = 1;
                self->asyncPending = 0;
            }
        }
    }
    if (self->state == 6) {
        self->dataLoaded = 1;
    }
    return started;
}
