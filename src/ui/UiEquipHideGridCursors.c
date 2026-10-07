// bdc 0x0895e2d4 UiEquipHideGridCursors
#include "bdc.h"

/* Hides the grid cursors of the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`):
   the cursor sprites `+0x516c`, `+0x517a` (random-button cursor), `+0x516e`, `+0x517c` and the
   extra highlight copy sprite (`data[+0x4fb4]`). */

void UiEquipHideGridCursors(UiEquip *self)
{
  GfxSprite **sprites;

  sprites = (GfxSprite **)(self->base).data;
  sprites[self->spriteIdx[6]]->flags &= ~1u;
  sprites = (GfxSprite **)(self->base).data;
  sprites[self->spriteIdx[0xd]]->flags &= ~1u;
  sprites = (GfxSprite **)(self->base).data;
  sprites[self->spriteIdx[7]]->flags &= ~1u;
  sprites = (GfxSprite **)(self->base).data;
  sprites[self->spriteIdx[0xe]]->flags &= ~1u;
  sprites = (GfxSprite **)(self->base).data;
  sprites[self->spriteCount]->flags &= ~1u;
}
