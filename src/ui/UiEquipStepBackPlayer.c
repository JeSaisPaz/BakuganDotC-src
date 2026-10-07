// bdc 0x0895e9ec UiEquipStepBackPlayer
#include "bdc.h"

/* Cancels back to the previous player on the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): frees the current model (`UiEquipFreeCurrentModel`), hides its gauge and
   stamp, decrements the current player `+0x4cdb` and the done count `+0x4cdc`, and puts the grid
   cursor (`+0x74`, `+0x75` = 0) back on the previous player's pick (reverse table
   `UiEquipMapBakuganIndex(screen, 1, id)`). */

void UiEquipStepBackPlayer(UiEquip *self)
{
    u8 player;
    u8 bakuganId;

    UiEquipFreeCurrentModel(self);
    player = self->editPlayer;
    bakuganId = UiEquipMapBakuganIndex(self, 0, self->gridCursor);
    UiEquipShowPlayerGauge(self, true, player, bakuganId);
    UiEquipStartPlayerStamp(self, self->editPlayer, 0);
    self->editPlayer = self->editPlayer - 1;
    self->doneCount = self->doneCount - 1;
    self->gridCursor = UiEquipMapBakuganIndex(self, 1, self->bakuganPick[self->editPlayer]);
    self->onRandom = 0;
}
