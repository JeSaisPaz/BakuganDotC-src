// bdc 0x089d0a44 NetCharaPushMessage
#include "bdc.h"

/* Appends one 0x28-byte (10-word) message record to the outgoing ring of a net character
   (`outBuf`, at most 5 records, fill count `outCount`), under the character's lock. While a previous
   push is latched (`msgPending`), it refuses unless `nextMsgSeq <= localSeq + 3` and fewer than 4
   records are queued. When accepted, the latch is set and, if the ring has room (`outCount < 5` and
   the record fits in 0x7cc bytes), the record is copied, the latch cleared, bit `0x10000000` of its
   second word set when `NetModeFlagIsClear` is true, its first word replaced by the incremented
   `nextMsgSeq`, `outCount` incremented, and the function returns 1. Otherwise it returns 0 (the
   latch stays set when the ring is full). */

bool NetCharaPushMessage(NetChara *self, u32 *msg)
{
    bool pushed = false;
    bool accept = true;
    u32 *rec;
    u32 off;
    int i;

    CoreLockAcquire(self->lock);
    if (self->msgPending != 0) {
        accept = false;
        if (self->nextMsgSeq <= (u32)self->localSeq + 3U && self->outCount < 4) {
            accept = true;
        }
    }
    if (accept) {
        self->msgPending = 1;
        if (self->outCount < 5) {
            off = self->outCount * 0x28;
            rec = (u32 *)(self->outBuf + off);
            if (off + 0x28 < 0x7cc) {
                self->msgPending = 0;
                for (i = 0; i < 10; i++) {
                    rec[i] = msg[i];
                }
                if (NetModeFlagIsClear()) {
                    rec[1] |= 0x10000000;
                }
                pushed = true;
                self->nextMsgSeq = self->nextMsgSeq + 1;
                rec[0] = self->nextMsgSeq;
                self->outCount = self->outCount + 1;
            }
        }
    }
    CoreLockRelease(self->lock);
    return pushed;
}
