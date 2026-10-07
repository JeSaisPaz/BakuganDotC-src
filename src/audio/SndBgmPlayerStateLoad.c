// bdc 0x089c4174 SndBgmPlayerStateLoad
#include "bdc.h"

/* State 2 handler (entry 2 of `g_sndBgmPlayerStateTable`): gets the `.at3` file named in `path`
   from CODataMng. Step 0 (only while the data manager exists) looks the name up among the live
   requests; with none, a new request is queued (stored in `fileHandle`, flags 4 set on it) and the
   step becomes 1 — if the file is already requested, nothing happens until it is done, then the
   request is shared (`fileHandle`), its size taken, and the image used directly (`buffer` = file
   data, step 999) or, for a caller-supplied buffer, copied into it with `CorePowerSafeMemcpy`
   (step 999, or 99 if the copy was refused). Step 1 waits until the request is done and then
   records the data pointer (only when `buffer` was the 0/1 heap sentinel) and size. Any other step
   (negative or ≥ 2) sets `loaded = 1` and moves to state 3 (prepare) when the command is 2 (play),
   else back to idle (state 0). Everything runs under the player lock. */

void SndBgmPlayerStateLoad(SndBgmPlayer *player)
{
  int step;
  IoData *data;
  IoData *req;

  CoreLockAcquire(player->lock);
  step = player->step;
  if (step == 0) {
    if (IoDataMngExists()) {
      data = IoDataMngFindByPath(IoGetDataMng(), player->path);
      if (data == NULL) {
        req = IoDataMngRequest(IoGetDataMng(), &player->fileHandle, player->path,
                               (uintptr_t)player->buffer, true, false);
        player->fileHandle = req;
        if (req != NULL) {
          IoDataAddFlags(req, 4);
          player->step = 1;
        }
      } else if (IoDataIsDone(data)) {
        player->fileHandle =
            IoDataMngRequest(IoGetDataMng(), &player->fileHandle, player->path, 0, false, false);
        player->bufferSize = IoDataGetUserData(data);
        if (!player->externalBuffer) {
          player->buffer = IoDataGetBuffer(data);
          player->step = 999;
        } else {
          void *dst = player->buffer;
          u32 size = player->bufferSize;
          int next = 99;
          CorePower *power = CorePowerGet();
          if (CorePowerSafeMemcpy(power, dst, IoDataGetBuffer(data), size, player->lock) != 0) {
            next = 999;
          }
          player->step = next;
        }
      }
    }
  } else if (step == 1) {
    if (player->fileHandle != NULL && IoDataIsDone(player->fileHandle)) {
      if ((uintptr_t)player->buffer <= 1) { /* 0/1 = heap-direction sentinel, no image yet */
        player->buffer = IoDataGetBuffer(player->fileHandle);
      }
      player->bufferSize = IoDataGetUserData(player->fileHandle);
      player->step = player->step + 1;
    }
  } else {
    player->loaded = 1;
    if (player->command == 2) {
      player->state = 3;
    } else {
      player->state = 0;
    }
    player->step = 0;
  }
  CoreLockRelease(player->lock);
}
