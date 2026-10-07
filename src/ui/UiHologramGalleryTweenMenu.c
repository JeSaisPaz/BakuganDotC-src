// bdc 0x0891f5ac UiHologramGalleryTweenMenu
#include "bdc.h"

/* Starts the open (`hide` 0) or close tweens of the command menu of the hologram gallery
   screen (task 391, `UiHologramGalleryCtor`). On open it first lays the menu out with
   `UiHologramGallerySetupMenu` for `menuMode` and shows sprite 5 on layer 8. Sprite 5 is
   always tweened (`UiTweenBegin`, scale 1.0, flags 1, tween slot = sprite index); sprites
   6..7 and 12..19 are tweened only while visible (`flags` bit 0), and on open their alpha is
   reset to 0 first. */

void UiHologramGalleryTweenMenu(UiHologramGallery *self, u8 hide)
{
  s32 i;

  if (hide == 0) {
    UiHologramGallerySetupMenu(self, self->menuMode);
    for (i = 5; i < 6; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 8;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
  } else {
    for (i = 5; i < 6; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
    }
  }
  for (i = 6; i < 8; i++) {
    GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];
    if ((u8)(sprite->flags & 1) != 0) {
      if (hide == 0) {
        sprite->alpha = 0.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      UiTweenBegin(1.0f, hide, sprite, &self->tweens[i], 1);
    }
  }
  for (i = 12; i < 20; i++) {
    GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];
    if ((u8)(sprite->flags & 1) != 0) {
      if (hide == 0) {
        sprite->alpha = 0.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      UiTweenBegin(1.0f, hide, sprite, &self->tweens[i], 1);
    }
  }
}
