// bdc 0x089616a0 UiEquipStartDoneSlotSpriteTween
#include "bdc.h"

/* Makes sprite `idx` of the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`) visible
   on layer 0x10 and starts its zoom tween (`UiTweenBegin` 1.5, mode 3). */

void UiEquipStartDoneSlotSpriteTween(UiEquip *self, u8 out, u16 idx)

{
  GfxSprite **sprites;

  sprites = (GfxSprite **)self->base.data;
  sprites[idx]->flags = sprites[idx]->flags | 1;
  ((GfxSprite **)self->base.data)[idx]->layerMask = 0x10;
  UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[idx], &self->tweens[idx], 3);
}
