// bdc 0x088d0b54 UiFieldHudUpdateHeadingArrow
#include "bdc.h"

/* Turns the heading arrow sprite 0xd of the field HUD (task 3001, `UiFieldHudCtor`; sprite array
   `base.data`, player `player`): heading = player Y rotation + atan2(viewDir) + pi, wrapped into
   (-pi, pi]; sets the sprite to scale 1 rotated by heading - pi/2 (`GfxSpriteSetScaleRotation`),
   then rebuilds its matrix (`GfxSpriteResetMatrix`) and post-multiplies it in place by a Z rotation
   of the same angle, setting flag 0x20 after each step. */

void UiFieldHudUpdateHeadingArrow(UiFieldHud *self)
{
  float heading;
  float angle;
  float t;
  float c;
  float s;
  float x;
  float y;
  float *m;
  s32 i;
  GfxSprite *sprite;

  heading = atan2f(self->viewDir[0], self->viewDir[1]);
  heading = self->player->base.base.rot[1] + heading + 3.1415927f;
  if (!(heading <= 3.1415927f)) {
    heading = heading - 6.2831855f;
  } else if (heading <= -3.1415927f) {
    heading = heading + 6.2831855f;
  }
  angle = heading - 1.5707964f;
  GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[0xd], 1.0f, 1.0f, angle, false);
  sprite = ((GfxSprite **)self->base.data)[0xd];
  sprite->flags |= 0x20;
  /* the same wrap test, re-run on the already wrapped heading */
  if (!(heading <= 3.1415927f)) {
    angle = (heading - 6.2831855f) - 1.5707964f;
  } else if (heading <= -3.1415927f) {
    angle = (heading + 6.2831855f) - 1.5707964f;
  }
  GfxSpriteResetMatrix(((GfxSprite **)self->base.data)[0xd]);
  sprite = ((GfxSprite **)self->base.data)[0xd];
  /* every matrix row (x, y, z, w) rotated about Z: (c*x - s*y, s*x + c*y, z, w); vrot takes quarter
     turns, so the angle is scaled by S703 = 2/pi */
  t = angle * 0.636619747f;
  c = VfCosQuarter(t);
  s = VfSinQuarter(t);
  m = sprite->matrix;
  for (i = 0; i < 4; i++) {
    x = m[i * 4 + 0];
    y = m[i * 4 + 1];
    m[i * 4 + 0] = c * x + -s * y;
    m[i * 4 + 1] = s * x + c * y;
  }
  sprite = ((GfxSprite **)self->base.data)[0xd];
  sprite->flags |= 0x20;
}
