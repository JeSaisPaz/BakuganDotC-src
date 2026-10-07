// bdc 0x08a2c89c GameQuestCamLookSpringSetLookAt
#include "bdc.h"

/* Entry 3 (`+0x1c`) of the look-at spring vtable `0x08af44cc` (`GameQuestCamLookSpringCtor`):
   copies the vec4 `v` into the look-at vector `+0x60`. */

void GameQuestCamLookSpringSetLookAt(GameQuestCamLookSpring *self, const float *v)

{
  self->point.x = v[0];
  self->point.y = v[1];
  self->point.z = v[2];
  self->point.w = v[3];
}
