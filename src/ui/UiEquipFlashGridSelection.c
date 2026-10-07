// bdc 0x0895e874 UiEquipFlashGridSelection
#include "bdc.h"

/* Starts the confirm flash (`UiFlashStart`, 2 frames) of the UiEquip Bakugan/gear loadout screen
   (task 302, `UiEquipCtor`): on the grid the hovered icon (`+0x5162 + +0x74`, slot 0) and its
   frame (`+0x5164 + +0x74`, slot 1); on the random button that button (`+0x5168`). */

void UiEquipFlashGridSelection(UiEquip *self)
{
    GfxSprite **sprites = (GfxSprite **)self->base.data;

    if (self->onRandom == 0) {
        UiFlashStart(2.0f, sprites[self->spriteIdx[1] + self->gridCursor], 0, 0);
        UiFlashStart(2.0f, sprites[self->spriteIdx[2] + self->gridCursor], 0, 1);
    } else {
        UiFlashStart(2.0f, sprites[self->spriteIdx[4]], 0, 0);
    }
}
