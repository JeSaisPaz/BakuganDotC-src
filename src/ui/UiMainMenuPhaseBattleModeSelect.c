// bdc 0x089a7b98 UiMainMenuPhaseBattleModeSelect
#include "bdc.h"

/* Phase 3 of `UiMainMenu` (battle entry), stepped by `phaseStep` after
   `UiMainMenuBobModels`: step 0 opens the battle-mode select (task 350,
   `UiBattleModeSelectCtor`) keeping the shared background, slides the title up and hides the
   battle sprites and the item label; step 1 waits for the slide; step 2 waits for task 350 to
   end; step 3 acts on `UiGetMenuResult`: 1 → opens the battle-rule select (task 340,
   `UiBattleRuleSelectCtor`) and goes to phase 4 step 2; 2 → opens the ad-hoc lobby task 1999
   and goes to phase 5 step 0; 0 (cancel) → slides the title back (step 4); other results wait.
   Step 4 waits for the slide back and restores the sprites and label (step 5); any step >= 5
   returns to phase 2 step 3. */

void UiMainMenuPhaseBattleModeSelect(UiMainMenu *self)
{
  s32 result;

  UiMainMenuBobModels(self);
  switch ((u32)self->base.phaseStep) {
  case 0:
    g_uiKeepSharedBg = 1;
    CoreTaskCreate(350, 100);
    UiMainMenuStartTitleSlide(self, 1);
    UiMainMenuSetBattleSpritesVisible(self, 0);
    UiMainMenuShowItemLabel(self, 0, (u8)self->cursor);
    self->base.phaseStep++;
    break;
  case 1:
    if (UiMainMenuStepTitleSlide(self, 1) == 1) {
      self->base.phaseStep++;
    }
    break;
  case 2:
    if (CoreTaskExists(350) == 0) {
      self->base.phaseStep = 3;
    }
    break;
  case 3:
    result = UiGetMenuResult(&self->base);
    if (result > 0) {
      if (result < 2) {
        g_uiKeepSharedBg = 1;
        CoreTaskCreate(340, 100);
        self->base.phase = 4;
        self->base.phaseStep = 2;
      } else if (result < 3) {
        g_uiKeepSharedBg = 1;
        CoreTaskCreate(1999, 100);
        self->base.phase = 5;
        self->base.phaseStep = 0;
      }
    } else if (result >= 0) {
      UiMainMenuStartTitleSlide(self, 0);
      self->base.phaseStep = 4;
    }
    break;
  case 4:
    if (UiMainMenuStepTitleSlide(self, 0) == 1) {
      UiMainMenuSetBattleSpritesVisible(self, 1);
      UiMainMenuShowItemLabel(self, 1, (u8)self->cursor);
      self->base.phaseStep = 5;
    }
    break;
  default:
    self->base.phase = 2;
    self->base.phaseStep = 3;
    break;
  }
}
