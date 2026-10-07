// bdc 0x0895ee84 UiEquipHideSelectionMarkers
#include "bdc.h"

/* Disables the selection markers of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): sets the skip flag `+0x4fb1` read by `UiEquipAnimateUnknownMarkers` and hides
   its four marker sprites (`+0x517e..+3`). */

void UiEquipHideSelectionMarkers(UiEquip *self)

{
  int i;
  GfxSprite *sprite;

  self->markersHidden = 1;
  i = 0;
  do {
    sprite = ((GfxSprite **)self->base.data)[self->spriteIdx[0xf] + i];
    i = i + 1;
    sprite->flags = sprite->flags & ~1u;
  } while (i < 4);
}
