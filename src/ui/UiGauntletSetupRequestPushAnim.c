// bdc 0x089352f8 UiGauntletSetupRequestPushAnim
#include "bdc.h"

/* Sets `+0x1b5c` of `UiGauntletSetup`, asking
   `UiGauntletSetupUpdateAvatarAnim` to play the avatar's `"12_editm_see_gauntlet_push"` motion.
    */

void UiGauntletSetupRequestPushAnim(UiGauntletSetup *self)

{
  self->pushRequested = '\x01';
  return;
}

