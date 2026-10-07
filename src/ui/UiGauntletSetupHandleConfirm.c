// bdc 0x08935594 UiGauntletSetupHandleConfirm
#include "bdc.h"

/* Handles Cross (pad `pressed` 0x4000) in `UiGauntletSetup`. Returns 0 when
   Cross was not pressed, 1 when the OK button is focused (`+0x74` ≠ 0) or a slot was toggled, 2
   when the focused slot `+0x76` holds no card (`+0x19f8 + slot` = −1). Toggling: if the slot's
   mark bit (`+0x19fc + slot` bit 0) is clear it requests the push animation
   (`UiGauntletSetupRequestPushAnim`) and marks it (`UiGauntletSetupToggleSlotMark``(1)`),
   otherwise unmarks it. */

s32 UiGauntletSetupHandleConfirm(UiGauntletSetup *self)

{
  u8 slot;
  
  if ((((self->base).pad)->pressed & 0x4000) == 0) {
    return 0;
  }
  if (self->focusArea != '\0') {
    return 1;
  }
  slot = self->item;
  if (self->slotCard[(char)slot] == 0xff) {
    return 2;
  }
  if ((self->slotMark[(char)slot] & 1) == 0) {
    UiGauntletSetupRequestPushAnim(self);
    UiGauntletSetupToggleSlotMark(self,'\x01',self->item);
    return 1;
  }
  UiGauntletSetupToggleSlotMark(self,'\0',slot);
  return 1;
}

