// bdc 0x089d1548 NetCharaSyncFrames
#include "bdc.h"

/* Builds lock-step frames in `g_netCharaSlots` (12 frames of two `NetCharaMsg` records) for a net
   character, under its lock (called from `NetCharaBuildPacket`):
   1. received records (`recvBuf`, `recvAckCount` of them) whose frame number is `frameCounter` + 1
      are appended as record 0 of a new frame (`slotCount`, at most 12), advancing `frameCounter`;
      `frameCounterOut` then takes the final `frameCounter`.
   2. outgoing ring records (`outBuf`, `outCount`) whose frame number is `localSeq` + 1 (stopping at
      one above `peerSeq`) are written as record 1 of the frame with that number, advancing
      `localSeq` and `readyFrames`. The frame search position carries over between ring records.
   3. consumed ring records are shifted out; `outHdr.count` and the length bits of
      `outHdr.lenFlags` get the remaining ring size.
   4. with ready frames, `g_netSyncState` is derived from the front frame's flag words, and when
      both records carry `0x1000000` the NetPlay flag `0x800000` is set and `syncCountdown` is
      started at 15 if it is 0. */

void NetCharaSyncFrames(NetChara *self)
{
    NetCharaMsg *out;
    NetCharaMsg *recv;
    NetCharaMsg (*frames)[2];
    u32 i;
    u32 j;
    u32 k;
    s32 consumed;
    u32 frame;
    NetPlay *netPlay;

    CoreLockAcquire(self->lock);
    out = (NetCharaMsg *)self->outBuf;
    recv = (NetCharaMsg *)self->recvBuf;
    frames = (NetCharaMsg (*)[2])g_netCharaSlots;

    for (i = 0; i < self->recvAckCount; i++) {
        if (self->slotCount >= 12) {
            break;
        }
        if (recv[i].frame == self->frameCounter + 1) {
            frames[self->slotCount][0] = recv[i];
            self->frameCounter = self->frameCounter + 1;
            self->slotCount = self->slotCount + 1;
        }
    }
    self->frameCounterOut = self->frameCounter;

    consumed = 0;
    j = 0;
    for (i = 0; i < self->outCount; i++) {
        frame = out[i].frame;
        if (frame != (u32)(self->localSeq + 1)) {
            continue;
        }
        if (self->peerSeq < frame) {
            break;
        }
        for (; j < self->slotCount; j++) {
            if (frames[j][0].frame == frame) {
                frames[j][1] = out[i];
                self->localSeq = self->localSeq + 1;
                self->readyFrames = self->readyFrames + 1;
                j++;
                consumed++;
                break;
            }
        }
        if ((u32)consumed >= self->outCount) {
            break;
        }
    }

    if (consumed > 0) {
        self->outCount = self->outCount - consumed;
        for (k = 0; k < self->outCount; k++) {
            out[k] = out[k + consumed];
        }
    }
    self->outHdr.count = (s8)self->outCount;
    self->outHdr.lenFlags =
        (u16)((self->outHdr.lenFlags & ~0x7ff) | ((u16)(self->outCount * 0x28) & 0x7ff));

    if (self->readyFrames != 0) {
        if (((frames[0][0].flags | frames[0][1].flags) & 0x30000000) == 0) {
            g_netSyncState = 2;
        } else {
            g_netSyncState = 0;
            if ((frames[0][1].flags & 0x10000000) == 0) {
                g_netSyncState = 1;
            }
        }
        if ((frames[0][0].flags & 0x1000000) != 0 && (frames[0][1].flags & 0x1000000) != 0) {
            if (NetPlayHasManager()) {
                netPlay = (NetPlay *)NetPlayGetManager();
                NetPlaySetFlags(netPlay, NetPlayGetFlags((NetPlay *)NetPlayGetManager()) | 0x800000);
            }
            if (self->syncCountdown == 0) {
                self->syncCountdown = 15;
            }
        }
    }
    CoreLockRelease(self->lock);
}
