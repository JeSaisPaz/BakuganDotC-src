// bdc 0x088b3d54 StopWallUpdateAll
#include "bdc.h"

/* Runs `StopWallUpdate` on every stop wall in `g_stopWallList`. Called from the stage update
   `ActorStageObjRecordUpdateAll` and from `BtlMainPhaseSceneOnly`. */

void StopWallUpdateAll(void)
{
  StopWall *self;
  StopWall *next;

  if (g_stopWallList != NULL && (self = (StopWall *)g_stopWallList->head) != NULL) {
    next = (StopWall *)self->base.next;
    while (1) {
      StopWallUpdate(self);
      if (next == NULL) break;
      self = next;
      next = (StopWall *)next->base.next;
    }
  }
}
