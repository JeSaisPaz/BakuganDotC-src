// bdc 0x0895cb1c UiEquipStartEmblemZoomIn
#include "bdc.h"

/* Makes the emblem sprite (`+0x5176`) of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`) visible and starts its zoom-in tween (`UiTweenBegin`, start scale 4.0, mode
   3). */

void UiEquipStartEmblemZoomIn(UiEquip *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[self->spriteIdx[0xb]]->flags |= 1;
  UiTweenBegin(4.0f, 0, sprites[self->spriteIdx[0xb]], &self->tweens[self->spriteIdx[0xb]], 3);
}
