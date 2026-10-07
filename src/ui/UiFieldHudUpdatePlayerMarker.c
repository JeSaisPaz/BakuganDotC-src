// bdc 0x088d09cc UiFieldHudUpdatePlayerMarker
#include "bdc.h"

/* Places the player marker (sprite 1) of the field HUD (task 3001, `UiFieldHudCtor`; sprite array
   `+0x1c`, player `+0x74`) at the radar centre (sprite 0 centre) plus the player's projected
   position (`UiFieldHudWorldToRadar`), rotates it to the view angle wrapped into (-pi, pi], and
   clears its linear-filter flag (0x20). */

void UiFieldHudUpdatePlayerMarker(UiFieldHud *self)

{
  float radarX;
  float radarY;
  float originX;
  float originY;
  float angle;
  GfxSprite *radar;
  GfxSprite *marker;

  radarX = 0.0f;
  radarY = 0.0f;
  UiFieldHudWorldToRadar(self->player->base.base.pos[0], self->player->base.base.pos[2], self,
                         &radarX, &radarY);
  radar = ((GfxSprite **)self->base.data)[0];
  originX = radar->posX;
  ((GfxSprite **)self->base.data)[1]->posX = originX + GfxSpriteGetWidth(radar) * 0.5f + radarX;
  radar = ((GfxSprite **)self->base.data)[0];
  originY = radar->posY;
  ((GfxSprite **)self->base.data)[1]->posY = originY + GfxSpriteGetHeight(radar) * 0.5f + radarY;
  angle = atan2f(self->viewDir[0], self->viewDir[1]) + 3.1415927f;
  if (!(angle <= 3.1415927f)) {
    angle = angle - 6.2831855f;
  } else if (angle <= -3.1415927f) {
    angle = angle + 6.2831855f;
  }
  GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[1], 1.0f, 1.0f, angle, false);
  marker = ((GfxSprite **)self->base.data)[1];
  marker->flags &= ~0x20u;
}
