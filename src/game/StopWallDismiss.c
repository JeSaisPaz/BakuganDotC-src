// bdc 0x088b3e54 StopWallDismiss
#include "bdc.h"

/* Walks the stop-wall list `g_stopWallList` (first node `*list`, next at `+4`) and sets state
   `+0x80 = 999` on every wall whose id `+0x50` equals `id`: `StopWallUpdate` then fades its
   effects out and destroys the wall 30 frames later. */

void StopWallDismiss(s32 id)

{
  StopWall *wall;
  StopWall *next;

  if (g_stopWallList != NULL && (wall = (StopWall *)g_stopWallList->head) != NULL) {
    for (;;) {
      next = (StopWall *)wall->base.next;
      if (wall->id == id) {
        wall->state = 999;
      }
      if (next == NULL) {
        break;
      }
      wall = next;
    }
  }
}
