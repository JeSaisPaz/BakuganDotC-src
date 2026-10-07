// bdc 0x089c3380 SndBgmPlayerLoadTrack
#include "bdc.h"

/* Selects `trackId` without starting playback: resolves its file name with
   `SndGetStreamFilePath`, copies it into `path`, and queues command 1 (load) with state 1
   (unload, then load). Only accepted while the player is idle (`state == 0`) and the id differs
   from the current one; returns true when queued (the player thread is then woken). An id without
   a file name falls back to track 0. */

bool SndBgmPlayerLoadTrack(SndBgmPlayer *player, s32 trackId)
{
    bool queued = false;
    s32 current;
    char *src;

    CoreLockAcquire(player->lock);
    current = player->trackId;
    if (current != trackId && player->state == 0) {
        if (player->prevTrackId != current && player->resumePrevious == 0) {
            player->prevTrackId = current;
        }
        player->trackId = 0;
        src = SndGetStreamFilePath(trackId);
        if (src != NULL) {
            player->trackId = trackId;
        } else {
            src = SndGetStreamFilePath(player->trackId);
        }
        strncpy(player->path, src, 0x80);
        queued = true;
        player->state = 1;
        player->command = 1;
        player->stopped = 0;
        player->loaded = 0;
    }
    CoreLockRelease(player->lock);
    if (queued) {
        BootWakeupThread(player->channel + 9);
    }
    return queued;
}
