// bdc 0x089c4394 SndBgmPlayerStatePrepare
#include "bdc.h"

/* State 3 handler (entry 3 of `g_sndBgmPlayerStateTable`): binds the loaded file to an Atrac
   decoder, one step per pass under the player lock; a failed call leaves the step unchanged so it
   is retried on the next pass. Step 0 flushes the data cache over the file image, acquires an
   Atrac id for ATRAC3+ (`sceAtracGetAtracID(0x1000)`, stored in `atracId` only on success) and
   `sceAtracSetData`; if that is rejected as the wrong codec (0x80630007) it releases the id and
   retries with plain ATRAC3 (0x1001); a negative result releases the id. Step 1 waits until
   `sceAtracIsSecondBufferNeeded` returns 0. Step 2 reads the loop/length information
   (`sceAtracGetSoundSample` into `endSample`/`loopStartSample`/`loopEndSample`, must return 0).
   Step 3 queries next sample and output channels (both must succeed) and remaining frames, then
   applies the loop setting: normally `loopActive = loopRequested` and `sceAtracSetLoopNum(-1)` for
   endless looping; when `resumePrevious` is set the flag is copied the other way
   (`loopRequested = loopActive`). Any other step (negative or ≥ 4) moves to state 4 (start). */

void SndBgmPlayerStatePrepare(SndBgmPlayer *player)
{
  int nextSample;
  u32 outputChannel;
  int remainFrame;
  int id;
  int ret;

  CoreLockAcquire(player->lock);
  switch (player->step) {
  case 0:
    sceKernelDcacheWritebackInvalidateRange(player->buffer, player->bufferSize);
    id = sceAtracGetAtracID(0x1000);
    if (id < 0) {
      break;
    }
    player->atracId = id;
    ret = sceAtracSetData(id, player->buffer, player->bufferSize);
    if (ret == (int)0x80630007) {
      sceAtracReleaseAtracID(player->atracId);
      player->atracId = sceAtracGetAtracID(0x1001);
      ret = sceAtracSetData(player->atracId, player->buffer, player->bufferSize);
    }
    if (ret < 0) {
      sceAtracReleaseAtracID(player->atracId);
    } else {
      player->step = player->step + 1;
    }
    break;
  case 1:
    if (sceAtracIsSecondBufferNeeded(player->atracId) == 0) {
      player->step = player->step + 1;
    }
    break;
  case 2:
    if (sceAtracGetSoundSample(player->atracId, &player->endSample, &player->loopStartSample,
                               &player->loopEndSample) == 0) {
      player->step = player->step + 1;
    }
    break;
  case 3:
    nextSample = 0;
    outputChannel = 0;
    remainFrame = 0;
    if (sceAtracGetNextSample(player->atracId, &nextSample) < 0) {
      break;
    }
    if (sceAtracGetOutputChannel(player->atracId, &outputChannel) < 0) {
      break;
    }
    sceAtracGetRemainFrame(player->atracId, &remainFrame);
    if (player->resumePrevious) {
      player->loopRequested = player->loopActive;
    } else {
      player->loopActive = player->loopRequested;
      if (player->loopActive) {
        sceAtracSetLoopNum(player->atracId, -1);
      }
    }
    player->step = player->step + 1;
    break;
  default:
    player->state = 4;
    player->step = 0;
    break;
  }
  CoreLockRelease(player->lock);
}
