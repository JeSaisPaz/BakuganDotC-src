// bdc 0x089d09ac NetCharaHasNewSyncedFrame
#include "bdc.h"

/* Checks the front frame of `g_netCharaSlots` under the character's lock: if the frame numbers of
   its two records agree (`slots[0] == slots[10]`) the number is stored through `outFrame` (0
   otherwise) and the function returns 1 when it is newer than the last committed frame
   (`lastCommitSeq`, see `NetCharaCommitFrame`). Its one caller is `NetPlayUpdate`, which uses
   the number to advance `NetPlay.maxFrameSeen`. */

bool NetCharaHasNewSyncedFrame(NetChara *self, s32 *outFrame)

{
  s32 *slots;
  s32 frame;
  bool newer;

  newer = false;
  frame = 0;
  CoreLockAcquire(self->lock);
  slots = (s32 *)g_netCharaSlots;
  if (slots[0] == slots[10]) {
    frame = slots[0];
    if (self->lastCommitSeq < frame) {
      newer = true;
    }
  }
  if (outFrame != NULL) {
    *outFrame = frame;
  }
  CoreLockRelease(self->lock);
  return newer;
}
