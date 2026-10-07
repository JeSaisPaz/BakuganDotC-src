// bdc 0x08910f84 UiPauseUpdatePlayerBadge
#include "bdc.h"

/* With `g_profileFlag0` set: for local player 1 (`NetGetLocalPlayerIndex`) shows the player
   badge sprite (entry 35, `+0x8c`, of the sprite table `data`) with alpha 1 and Z -100, centres its
   pivot, resets scale 1 / angle 0 and shows help line message 5 6 px above it
   (`UiHelpLineShow`); for any other player hides the help line (`UiHelpLineHide`) and the
   badge. With the flag clear nothing is touched. */

void UiPauseUpdatePlayerBadge(UiPause *self)
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
      UiHelpLineHide();
      ((GfxSprite **)self->base.data)[35]->flags &= ~1u;
    }
  }
}
