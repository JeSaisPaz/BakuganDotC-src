// bdc 0x0891109c UiPauseInitPlayerBadge
#include "bdc.h"

/* With `g_profileFlag0` set, shows the player badge sprite (entry 35, `+0x8c`, of the sprite table
   `data`): visible, alpha 1, z = -100, centred pivot, scale 1 / angle 0, then puts help line 5
   (local player 1, `NetGetLocalPlayerIndex`) or 6 (otherwise) 6 px above its centre
   (`UiHelpLineShow`). Does nothing when the flag is clear. */

void UiPauseInitPlayerBadge(UiPause *self)
{
  GfxSprite *badge;

  if (SaveGetProfileFlag0() != 0) {
    if (NetGetLocalPlayerIndex() == 1) {
      ((GfxSprite **)self->base.data)[35]->flags |= 1;
      ((GfxSprite **)self->base.data)[35]->alpha = 1.0f;
      ((GfxSprite **)self->base.data)[35]->posZ = -100.0f;
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[35]);
      ((GfxSprite **)self->base.data)[35]->flags |= 0x20;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[35], 1.0f, 1.0f, 0.0f);
      badge = ((GfxSprite **)self->base.data)[35];
      UiHelpLineShow(badge->posX, badge->posY - 6.0f, 5);
    } else {
      ((GfxSprite **)self->base.data)[35]->flags |= 1;
      ((GfxSprite **)self->base.data)[35]->alpha = 1.0f;
      ((GfxSprite **)self->base.data)[35]->posZ = -100.0f;
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[35]);
      ((GfxSprite **)self->base.data)[35]->flags |= 0x20;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[35], 1.0f, 1.0f, 0.0f);
      badge = ((GfxSprite **)self->base.data)[35];
      UiHelpLineShow(badge->posX, badge->posY - 6.0f, 6);
    }
  }
}
