// bdc 0x08993ddc UiUnlockCodeCheckPhase
#include "bdc.h"

/* Phase 4 of `UiUnlockCode` (entered with OK): fades the keyboard out
   (`UiUnlockCodeStartPanelFade`/`UiUnlockCodePanelFadeDone`), normalises and validates the code
   (`UiUnlockCodeNormalizeCode`, `UiUnlockCodeValidateCode` → `validateResult`). A valid 8-digit
   code sets profile `playthroughClearBits` bit 2 and, after its message, switches the screen to
   10-digit mode (`dimensionsMode` = 1, `digitCount` = 10); a valid 10-digit code is registered
   (`UiUnlockCodeRegisterCode`), stores reward[1] + 1 in profile word 0x14, sets
   `g_uiKeepSharedBg` and shows the reward task 0x177 (`CoreTaskCreateDefault`, waits until it
   ends) before exiting (phase 5). Otherwise shows message 0xb (valid) / 0xd (invalid)
   (`UiUnlockCodeSetMessage`, `UiUnlockCodeShowMessage`), resets the input and fades the
   keyboard back in, returning to phase 3. */

void UiUnlockCodeCheckPhase(UiUnlockCode *self)
{
  SaveProfile *profile;

  switch (self->base.phaseStep) {
  case 0:
    UiUnlockCodeStartPanelFade(self, 1);
    self->base.phaseStep++;
    break;
  case 1:
    if (UiUnlockCodePanelFadeDone(self, 1) == 1) {
      self->base.phaseStep++;
    }
    break;
  case 2:
    UiUnlockCodeNormalizeCode(self);
    self->validateResult = (u8)UiUnlockCodeValidateCode(self);
    if (self->validateResult == 1) {
      if (self->dimensionsMode == 1) {
        self->base.phaseStep = 7;
        return;
      }
      profile = SaveGetProfile();
      profile->data->playthroughClearBits |= 4;
    }
    self->base.phaseStep++;
    break;
  case 3:
    /* both dimensionsMode branches are identical in the binary */
    if (self->validateResult == 1) {
      UiUnlockCodeSetMessage(self, 0xb);
    } else {
      UiUnlockCodeSetMessage(self, 0xd);
    }
    self->base.phaseStep++;
    break;
  case 4:
    if (UiUnlockCodeShowMessage(self) == 1) {
      if (self->validateResult == 1 && self->dimensionsMode == 0) {
        self->dimensionsMode = 1;
        self->digitCount = 10;
      }
      self->base.phaseStep = 5;
    }
    break;
  case 5:
    UiUnlockCodeResetInput(self);
    UiUnlockCodeStartPanelFade(self, 0);
    self->base.phaseStep++;
    break;
  case 6:
    if (UiUnlockCodePanelFadeDone(self, 0) == 1) {
      UiUnlockCodeLayoutKeyboard(self);
      self->base.phaseStep = 0;
      self->introTimer = 0;
      self->base.phase = 3;
    }
    break;
  case 7:
    UiUnlockCodeRegisterCode(self);
    self->base.phaseStep++;
    break;
  case 8:
    g_uiKeepSharedBg = 1;
    profile = SaveGetProfile();
    SaveProfileSetWord(profile, 0x14, self->reward[1] + 1);
    CoreTaskCreateDefault(0x177, (void *)(uintptr_t)(u8)self->reward[2]);
    self->base.phaseStep++;
    break;
  case 9:
    g_uiKeepSharedBg = 1;
    if (CoreTaskExists(0x177) == 0) {
      self->base.phaseStep = 10;
    }
    break;
  case 10:
    self->base.phase = 5;
    self->base.phaseStep = 0;
    break;
  }
}
