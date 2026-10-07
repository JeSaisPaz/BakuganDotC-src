// bdc 0x089c3f30 SndBgmPlayerStateUnload
#include "bdc.h"

/* State 1 handler (entry 1 of `g_sndBgmPlayerStateTable`): tears down the current stream and
   picks the next state. Step 0 waits for the decoder to be silent (a decoder that is still audible
   is faded to 0 over 0.5 s on channel 0, or switched off at once on channel 1; with a suspend
   pending it just waits for the decoder to be quiescent). Step 1 releases the Atrac id
   (`sceAtracReleaseAtracID`), releases the CODataMng file handle and, unless the buffer is
   caller-supplied, resets `buffer` to its default allocation flag. Step 2 selects the next state:
   for command 1/2 (load / play) state 2 (load); for command 3 (stop) state 0, `trackId = -1`, and,
   if `resumePrevious` is set and `prevTrackId != -1`, restarts the previous track (command 2, state
   2, file name from `SndGetStreamFilePath`). */

void SndBgmPlayerStateUnload(SndBgmPlayer *player)
{
    s32 step;
    s32 command;
    s32 prev;

    CoreLockAcquire(player->lock);
    step = player->step;
    if (step == 0) {
        if (player->suspendRequested != 0) {
            if (player->decoderQuiesced != 0) {
                player->step = step + 1;
            }
        } else if (SndDecOutExists(player->channel) == 0) {
            player->step = player->step + 1;
        } else if (SndDecOutIsFadeDone(SndDecOutGet(player->channel), 1) != 0) {
            /* already silent: stop decoding and reset the fade state */
            SndDecOutSetDecodeEnabled(SndDecOutGet(player->channel), 0);
            SndDecOutResetVolume(SndDecOutGet(player->channel));
            player->step = player->step + 1;
        } else if (SndDecOutIsFadeDone(SndDecOutGet(player->channel), 0) != 0) {
            /* still audible and no fade running */
            if (player->channel == 0) {
                SndDecOutSetVolumeF(0.0f, 0.5f, SndDecOutGet(0));
            } else {
                SndDecOutSetDecodeEnabled(SndDecOutGet(player->channel), 0);
                SndDecOutResetVolume(SndDecOutGet(player->channel));
                player->step = player->step + 1;
            }
        }
    } else if (step == 1) {
        if (player->atracId >= 0) {
            sceAtracReleaseAtracID(player->atracId);
        }
        player->atracId = -1;
        if (player->fileHandle != NULL) {
            IoDataMngRelease(IoGetDataMng(), &player->fileHandle, player->fileHandle);
            player->fileHandle = NULL;
            if (player->externalBuffer == 0) {
                /* 0/1 = heap-direction sentinel for the next load */
                if (player->loadFromLow != 0) {
                    player->buffer = (void *)(uintptr_t)1;
                } else {
                    player->buffer = NULL;
                }
            }
        }
        player->step = player->step + 1;
    } else {
        command = player->command;
        if (command >= 1 && command <= 2) {
            player->state = 2;
        } else {
            player->state = 0;
            player->trackId = -1;
            if (player->resumePrevious != 0) {
                prev = player->prevTrackId;
                player->resumePrevious = 0;
                if (prev != -1) {
                    player->command = 2;
                    player->state = 2;
                    player->trackId = prev;
                    strncpy(player->path, SndGetStreamFilePath(prev), 0x80);
                }
            }
        }
        player->step = 0;
    }
    CoreLockRelease(player->lock);
}
