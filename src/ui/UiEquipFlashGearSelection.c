// bdc 0x08960ba8 UiEquipFlashGearSelection
#include "bdc.h"

/* Starts the confirm flash (`UiFlashStart`, 2 frames) in the equipment panel of the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`): on the list row the current player's
   entry sprite (`+0x51aa + +0x51ac*player + entry`, slot 0) and its icon (`+0x51ce + +0x51d0*player
   + entry`, slot 1); on the OK row the OK button (`+0x519e + +0x51a0*player`); the active row is
   `+0x5025` (in network mode — `SaveGetProfileFlag0` set — the local player's row
   `+0x5026[+0x52a0]`); row 0 = equipment list, row 1 = OK button. */

void UiEquipFlashGearSelection(UiEquip *self)
{
    s8 row = self->activeRow;
    GfxSprite **sprites;
    s8 player;

    if (SaveGetProfileFlag0() != 0)
        row = (s8)self->gearRowFlags[*(s32 *)self->localPlayer];
    sprites = (GfxSprite **)self->base.data;
    player = self->editPlayer;

    if (row == 0) {
        UiFlashStart(2.0f,
                     sprites[self->spriteIdx[0x25] + self->spriteIdx[0x26] * player +
                             self->panelEntry],
                     0, 0);
        sprites = (GfxSprite **)self->base.data;
        UiFlashStart(2.0f,
                     sprites[self->spriteIdx[0x37] + self->spriteIdx[0x38] * self->editPlayer +
                             self->panelEntry],
                     0, 1);
    } else {
        UiFlashStart(2.0f, sprites[self->spriteIdx[0x1f] + self->spriteIdx[0x20] * player], 0, 0);
    }
}
