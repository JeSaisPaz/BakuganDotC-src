// bdc 0x08921784 UiHologramGalleryTweenHelpPanel
#include "bdc.h"

/* Starts the tweens (mode 1) of the help-panel sprites listed by
   `UiHologramGalleryHelpPanelSprite`, showing them first, or reverses them when `hide` is set. */

void UiHologramGalleryTweenHelpPanel(UiHologramGallery *self, u8 hide)

{
  GfxSprite **sprites;
  u32 n = 0;
  u32 id;

  while ((id = UiHologramGalleryHelpPanelSprite(self, n & 0xff)) != 0xff) {
    sprites = (GfxSprite **)self->base.data;
    if (hide == 0) {
      sprites[id]->flags |= 1;
      sprites = (GfxSprite **)self->base.data;
    }
    UiTweenBegin(1.0f, hide, sprites[id], &self->tweens[id], 1);
    n++;
  }
  return;
}
