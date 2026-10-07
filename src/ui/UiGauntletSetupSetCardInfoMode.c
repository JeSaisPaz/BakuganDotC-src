// bdc 0x089349dc UiGauntletSetupSetCardInfoMode
#include "bdc.h"

/* Resets the 0x10-byte card-info animation state `+0x1a68` of
   `UiGauntletSetup` and sets its mode byte: 1 = open (show the focused card's
   name/help), 2 = close; any other value leaves it 0 (inactive). */

void UiGauntletSetupSetCardInfoMode(UiGauntletSetup *self, u8 mode)

{
  u8 *s;
  
  s = self->cardInfo;
  if (mode < 2) {
    if (mode != '\0') {
      memset(s,0,0x10);
      self->cardInfo[0] = mode;
      return;
    }
  }
  else if (mode < 3) {
    memset(s,0,0x10);
    self->cardInfo[0] = mode;
    return;
  }
  memset(s,0,0x10);
  return;
}

