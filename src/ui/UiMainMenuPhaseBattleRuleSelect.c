// bdc 0x089a7de4 UiMainMenuPhaseBattleRuleSelect
#include "bdc.h"

/* Battle-rule select phase of `UiMainMenu` (bobbing the item models each frame):
   step 0 sets `g_uiKeepSharedBg`, opens the battle-rule select (task 340,
   `UiBattleRuleSelectCtor`), slides the title up and hides the battle sprites and item label;
   step 1 waits for the slide; step 2 waits for task 340 to end; step 3 acts on
   `UiGetMenuResult`: 1/2/3 store 0/1/2 into profile word 0x4a and set `leave` (step 5);
   4 sets `leaveAlt` and `leave` (step 5); 0 (cancel) sets `g_uiKeepSharedBg` and, when profile
   flag 0 is clear, reopens the battle-mode select (task 350, phase 3 step 2), else slides the title
   back (phase 3 step 4) with the menu flag word and profile flags 0x7eff cleared; results above 4
   do nothing. Step 4 waits for the slide back and re-shows the sprites (step 5). Any step above 4
   switches to phase 2 step 6. */

void UiMainMenuPhaseBattleRuleSelect(UiMainMenu *self)
{
  SaveProfile *profile;

  UiMainMenuBobModels(self);
  switch ((u32)self->base.phaseStep) {
  case 0:
    g_uiKeepSharedBg = 1;
    CoreTaskCreate(0x154, 100);
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
    if (CoreTaskExists(0x154) == 0) {
      self->base.phaseStep = 3;
    }
    break;
  case 3:
    UiHelpLineHide();
    switch ((u32)UiGetMenuResult(&self->base)) {
    case 0:
      g_uiKeepSharedBg = 1;
      if (SaveGetProfileFlag0() == 0) {
        CoreTaskCreate(0x15e, 100);
        UiMainMenuSetNetSpritesVisible(self, 1);
        self->base.phase = 3;
        self->base.phaseStep = 2;
      } else {
        UiMainMenuStartTitleSlide(self, 0);
        self->base.phase = 3;
        self->base.phaseStep = 4;
        UiMainMenuSetNetSpritesVisible(self, 1);
        UiMenuFlagsModify(2, 0);
        if (SaveHasProfile()) {
          SaveProfileClearFlags(SaveGetProfile(), 0x7eff);
        }
        SaveRefreshProfileFlag0();
      }
      break;
    case 1:
      profile = SaveGetProfile();
      profile->words[0x4a] = 0;
      self->leave = 1;
      self->base.phaseStep = 5;
      break;
    case 2:
      profile = SaveGetProfile();
      profile->words[0x4a] = 1;
      self->leave = 1;
      self->base.phaseStep = 5;
      break;
    case 3:
      profile = SaveGetProfile();
      profile->words[0x4a] = 2;
      self->leave = 1;
      self->base.phaseStep = 5;
      break;
    case 4:
      self->leaveAlt = 1;
      self->leave = 1;
      self->base.phaseStep = 5;
      break;
    }
    break;
  case 4:
    if (UiMainMenuStepTitleSlide(self, 0) == 1) {
      UiMainMenuSetBattleSpritesVisible(self, 1);
      UiMainMenuShowItemLabel(self, 1, (u8)self->cursor);
      UiMainMenuSetNetSpritesVisible(self, 1);
      self->base.phaseStep = 5;
    }
    break;
  default:
    self->base.phase = 2;
    self->base.phaseStep = 6;
    break;
  }
}
