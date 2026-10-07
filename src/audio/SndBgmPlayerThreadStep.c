// bdc 0x089c3b70 SndBgmPlayerThreadStep
#include "bdc.h"

/* One iteration of a player thread (`MyThread-Sound-Bgm0/1`, called in a loop by
   `SndBgmThreadMain`): handles the PSP suspend/resume handshake, runs the state handler of the
   current `state` through the member-function table `g_sndBgmPlayerStateTable`, forwards a
   pending volume request to the decoder, waits one vblank, and, once the sound manager's shutdown
   flag is set, waits for the sibling sound threads to finish and ends its own thread. Returns
   1 (continue) as long as the shutdown flag is clear. */

s32 SndBgmPlayerThreadStep(SndBgmPlayer *player)
{
    s32 runHandler = 1;
    s32 resumed = 0;
    s32 sleep = 0;
    s32 channel;
    s32 state;
    u8 volumePending;
    u8 suspendRequested;
    s32 percent;
    s32 fadeMs;
    s32 allDone;
    s32 i;
    s32 slot;

    CoreLockAcquire(player->lock);
    channel = player->channel;
    CoreLockRelease(player->lock);
    CoreLockAcquire(player->lock);
    if (player->suspendRequested != 0) {
        runHandler = 0;
        if (player->suspended == 0 && player->decoderQuiesced != 0) {
            if (player->bufferVolatile == 0) {
                player->suspended = 1;
                sleep = 1;
            } else if (player->bufferReleased == 0) {
                CorePowerVolatileFree(CorePowerGet(), player->buffer);
                player->bufferReleased = 1;
                player->suspended = 1;
                sleep = 1;
            }
        }
        if (player->resumeRequested != 0) {
            player->suspendRequested = 0;
            player->resumeRequested = 0;
            if (player->state == 0) {
                if (player->command == 2) {
                    player->state = 1;
                    player->step = 0;
                }
            } else {
                player->state = 1;
                player->step = 0;
            }
            resumed = 1;
            if (player->fileHandle != NULL) {
                if (!IoDataIsDone(player->fileHandle)) {
                    IoDataAddFlags(player->fileHandle, 0x80000000);
                } else if (player->bufferVolatile != 0) {
                    void *buffer = player->buffer;

                    if (buffer != IoDataGetBuffer(player->fileHandle)) {
                        void *dst = player->buffer;
                        void *src = IoDataGetBuffer(player->fileHandle);

                        memcpy(dst, src, player->bufferSize);
                    }
                }
            }
        }
    }
    state = player->state;
    if (runHandler && player->bufferVolatile != 0 && player->bufferReleased != 0) {
        runHandler = 0;
    }
    CoreLockRelease(player->lock);

    if (runHandler) {
        const MemberFnPtr *e = &g_sndBgmPlayerStateTable[state];
        u8 *obj = (u8 *)player + e->delta;
        void *fn = e->pfn;

        if (e->index != 0) {
            const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)e->pfn);
            const VtblEntry *entry = &vtbl[e->index];

            obj += entry->delta;
            fn = entry->fn;
        }
        ((void (*)(void *))fn)(obj);
    }

    if (sleep) {
        BootSleepCurrentThread();
    } else {
        CoreLockAcquire(player->lock);
        volumePending = player->volumePending;
        suspendRequested = player->suspendRequested;
        percent = player->pendingVolume;
        player->volumePending = 0;
        fadeMs = player->pendingFadeMs;
        CoreLockRelease(player->lock);
        if (suspendRequested == 0) {
            if (resumed && SndDecOutExists(channel) != 0) {
                SndDecOutRequestResume(SndDecOutGet(channel));
            }
            if (volumePending != 0 && state != 1 && SndDecOutExists(channel) != 0) {
                SndDecOutSetVolume(SndDecOutGet(channel), percent, fadeMs);
            }
        }
    }
    sceDisplayWaitVblankStartCB();

    for (;;) {
        if (SndGetManager()->stopFlag == 0) {
            return 1;
        }
        allDone = 1;
        sceDisplayWaitVblankStartCB();
        if (channel == 0) {
            /* Channel 0 waits for the channel-1 player (slot 10) and decoder (slot 7). */
            for (i = 1; i > 0; i--) {
                slot = i + 9;
                if (BootIsThreadRunning(slot) != 0) {
                    allDone = 0;
                    if (BootIsThreadSleeping(slot)) {
                        BootWakeupThread(slot);
                    }
                    break;
                }
                slot = i + 6;
                if (BootIsThreadRunning(slot) != 0) {
                    allDone = 0;
                    if (BootIsThreadSleeping(slot)) {
                        BootWakeupThread(slot);
                    }
                    break;
                }
            }
        }
        if (allDone) {
            if (BootIsThreadRunning(channel + 6) != 0) {
                if (BootIsThreadSleeping(channel + 6)) {
                    BootWakeupThread(channel + 6);
                }
            } else {
                BootDeleteThread(channel + 9);
            }
        }
    }
}
