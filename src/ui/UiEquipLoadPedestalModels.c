// bdc 0x08957274 UiEquipLoadPedestalModels
#include "bdc.h"

/* Creates the pedestal model of every player slot of `UiEquip` (`+0x4cda` players,
   `UiEquipLoadPedestalModel`). */

void UiEquipLoadPedestalModels(UiEquip *self)
{
    int i;

    for (i = 0; i < self->playerCount; i++) {
        UiEquipLoadPedestalModel(self, (u8)i);
    }
}
