// bdc 0x08959c90 UiEquipSetupPhase
#include "bdc.h"

/* Phase 1 of `UiEquip`: step 0 builds everything (`UiEquipCreateSprites`,
   `UiEquipInitModelSlots`, `UiEquipInitSpriteIndexTable`, `UiEquipInitGridOffsets`,
   `UiEquipInitPlayerMarkers`, `UiEquipInitPanelOffsets`, `UiEquipInitSlotSprites`, the two
   text printers); step 1 waits for the fade and starts `"main_start.fab"`; any other step clears
   `float4f7c` and enters the offline main phase 2, or in network mode phase 5
   (`UiEquipNetMainPhase`) after setting the net manager flags 0x1000000/0x2000000
   (`NetPlayClearFlags`, `NetPlaySetFlags`) and resetting net character sync. */

void UiEquipSetupPhase(UiEquip *self)
{
    switch (self->base.phaseStep) {
    case 0:
        UiEquipCreateSprites(self);
        UiEquipInitModelSlots(self);
        UiEquipInitSpriteIndexTable(self);
        UiEquipInitGridOffsets(self);
        UiEquipInitPlayerMarkers(self);
        UiEquipInitPanelOffsets(self);
        UiEquipInitSlotSprites(self);
        UiEquipCreateNameText(self);
        UiEquipCreateHelpText(self);
        self->base.phaseStep = self->base.phaseStep + 1;
        return;
    case 1:
        if (!GfxFaderIsFinished(GfxGetActiveFader())) {
            return;
        }
        UiSharedAnimStart(300.0f, 0.0f, 0.0f, self, (void *)"main_start.fab", 1, 0);
        self->base.phaseStep = self->base.phaseStep + 1;
        return;
    default:
        break;
    }

    self->float4f7c = 0.0f;
    if (SaveGetProfileFlag0() == 0) {
        self->base.phaseStep = 0;
        self->base.phase = self->base.phase + 1;
    } else {
        self->base.phase = 5;
        self->base.phaseStep = 0;
        if (NetPlayHasManager()) {
            NetPlayClearFlags(NetPlayGetManager(), 0x1000000);
            NetPlaySetFlags(NetPlayGetManager(), 0x2000000);
            NetCharaResetAllSync();
        }
    }
}
