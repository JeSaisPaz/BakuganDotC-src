// bdc 0x0895c67c UiEquipCardEquipPhase
#include "bdc.h"

/* Phase 3 (phase table `0x08a9d5f8`) of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): after all players picked their Bakugan it opens the ability-card loadout
   sub-screen. Step 0 runs `UiEquipStoreSelectionWords`; step 1 sets `g_equipPendingFlag` and
   `g_uiKeepSharedBg` and creates task 303 (`UiCardEquipCtor`, `CoreTaskCreate`); step 2 waits
   until it is gone (`CoreTaskExists`), clears `g_equipPendingFlag` and goes to step 3; steps 3+
   (and negative steps) read its menu result (`UiGetMenuResult`): 0 = back, returns to the grid
   (phase 2 step 5) with `doneCount` decremented (`UiEquipResetCursor`); 1 = accepted, closes
   the pedestals (`UiEquipSetupSlotModels`) and goes to the close steps (phase 2 step 7); any
   other result keeps waiting. Every frame it also runs the animation and layout helpers. */

void UiEquipCardEquipPhase(UiEquip *self)
{
    s32 step;
    s32 result;

    UiEquipUpdateAnimations(self);
    step = self->base.phaseStep;
    if (step == 0) {
        UiEquipStoreSelectionWords(self);
        self->base.phaseStep++;
    } else if (step == 1) {
        g_equipPendingFlag = 1;
        g_uiKeepSharedBg = 1;
        CoreTaskCreate(0x12f, 100);
        self->base.phaseStep++;
    } else if (step == 2) {
        if (CoreTaskExists(0x12f) == 0) {
            g_equipPendingFlag = 0;
            self->base.phaseStep = 3;
        }
    } else {
        result = UiGetMenuResult(&self->base);
        if (result == 0) {
            UiEquipResetCursor(self);
            self->doneCount--;
            self->base.phase = 2;
            self->base.phaseStep = 5;
        } else if (result == 1) {
            UiEquipSetupSlotModels(self, true);
            self->base.phase = 2;
            self->base.phaseStep = 7;
        }
    }
    UiEquipLayoutGrid(self);
    UiEquipLayoutPlayerMarkers(self);
    UiEquipPulsePendingPlayers(self);
}
