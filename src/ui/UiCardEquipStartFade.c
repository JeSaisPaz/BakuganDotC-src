// bdc 0x0896bae0 UiCardEquipStartFade
#include "bdc.h"

/* Resets the fade record of `UiCardEquip` (`+0x29dc`, 0x10 bytes) for fading in
   (`closing` = 0) or out (start level 0.8). */

void UiCardEquipStartFade(UiCardEquip *self, u8 closing)

{
  memset(&self->fadeOn,0,0x10);
  self->fadeOn = 1;
  if (closing != 0) {
    self->fade = 0.8f;
    self->fadeTarget = 0.8f;
  }
}
