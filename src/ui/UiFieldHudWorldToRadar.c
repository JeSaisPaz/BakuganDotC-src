// bdc 0x088d0958 UiFieldHudWorldToRadar
#include "bdc.h"

/* Maps world (`x`, `z`) to radar coordinates of the field HUD (task 3001, `UiFieldHudCtor`;
   sprite array `+0x1c`, player `+0x74`): offset by the map origin `+0x88/+0x8c`, scaled by
   `+0x90/+0x94` over the map size `+0x80/+0x84`, minus the centre `+0xb4/+0xb8`, then rotated by
   the view (`sin +0xc0`, `cos +0xc4`) into `*outX`, `*outY`. */

void UiFieldHudWorldToRadar(float x, float z, UiFieldHud *self, float *outX, float *outY)

{
  float dx;
  float dz;

  dx = ((x - self->mapRect[2]) * self->radarSize[0]) / self->mapRect[0] - self->radarHalf[0];
  dz = ((z - self->mapRect[3]) * self->radarSize[1]) / self->mapRect[1] - self->radarHalf[1];
  *outX = dx * self->viewDir[1] - dz * self->viewDir[0];
  *outY = dx * self->viewDir[0] + dz * self->viewDir[1];
  return;
}
