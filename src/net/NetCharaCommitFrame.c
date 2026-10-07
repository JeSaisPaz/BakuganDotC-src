// bdc 0x089d0d44 NetCharaCommitFrame
#include "bdc.h"

/* Consumes the front frame of the net character's slot queue, if `ready` (see `NetCharaSetReady`)
   is set: stores the front frame number (word 0 of `g_netCharaSlots`) in `lastCommitSeq`, then,
   when `readyFrames` and `slotCount` are both non-zero, shifts the 12 two-record frames (20 words
   each) of `g_netCharaSlots` down by one frame and decrements both counters; finally clears
   `ready`. Returns `readyFrames` (after the decrement, if any), or -1 when `ready` was not set.
   Runs under the character's lock. */

s32 NetCharaCommitFrame(NetChara *self)
{
    s32 result = -1;
    u32 *slots;
    int frame;
    int i;

    CoreLockAcquire(self->lock);
    if (self->ready != 0) {
        result = self->readyFrames;
        self->lastCommitSeq = *(s32 *)g_netCharaSlots;
        if (result != 0 && self->slotCount != 0) {
            /* frames 1..11 move to 0..10; each frame is two 10-word records */
            for (frame = 1; frame < 12; frame++) {
                slots = (u32 *)g_netCharaSlots + (frame - 1) * 20;
                for (i = 0; i < 20; i++) {
                    slots[i] = slots[i + 20];
                }
            }
            result = self->readyFrames - 1;
            self->readyFrames = result;
            self->slotCount = self->slotCount - 1;
        }
        self->ready = 0;
    }
    CoreLockRelease(self->lock);
    return result;
}
