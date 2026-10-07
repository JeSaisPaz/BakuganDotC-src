// bdc 0x088f8250 GameQuestCamPointModeGetPoint
#include "bdc.h"

/* Vtable `0x08af4454` slot 7 of the point camera mode: copies the entry's point
   (`entry->point`) to `out`. */

void GameQuestCamPointModeGetPoint(GameQuestCamPointMode *self, float *out)

{
  const GameQuestCamModeEntry *entry = self->entry;

  out[0] = entry->point.x;
  out[1] = entry->point.y;
  out[2] = entry->point.z;
  out[3] = entry->point.w;
}
