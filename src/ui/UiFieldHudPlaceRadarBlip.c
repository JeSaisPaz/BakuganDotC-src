// bdc 0x088d0d08 UiFieldHudPlaceRadarBlip
#include "bdc.h"

/* Positions blip sprite `index` of the field HUD (task 3001, `UiFieldHudCtor`; sprite array
   `+0x1c`, player `+0x74`) relative to the player marker (sprite 1) by the projected world
   position `pos` (`UiFieldHudWorldToRadar`) and, except for index 14, rotates it to the view
   and sets its linear-filter flag (0x20). */

void UiFieldHudPlaceRadarBlip(UiFieldHud *self, float *pos, s32 index)

{
  float radarX;
  float radarY;
  float angle;
  GfxSprite *blip;

  radarX = 0.0f;
  radarY = 0.0f;
  UiFieldHudWorldToRadar(pos[0], pos[2], self, &radarX, &radarY);
  ((GfxSprite **)self->base.data)[index]->posX =
      ((GfxSprite **)self->base.data)[1]->posX - radarX;
  ((GfxSprite **)self->base.data)[index]->posY =
      ((GfxSprite **)self->base.data)[1]->posY - radarY;
  if (index != 14) {
    angle = atan2f(self->viewDir[0], self->viewDir[1]);
    GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[index], 1.0f, 1.0f,
                              (angle + 3.1415927f) - 1.5707964f, false);
    blip = ((GfxSprite **)self->base.data)[index];
    blip->flags |= 0x20;
  }
}
