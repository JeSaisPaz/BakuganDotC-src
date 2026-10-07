// bdc 0x08970408 UiOptionStartFade
#include "bdc.h"

/* Resets the fade record of `UiOption` (`+0xb8c`, 0x10 bytes) for fading in
   (`closing` = 0) or out (start level 0.8). */

void UiOptionStartFade(UiOption *self, u8 closing)

{
  if (closing == '\0') {
    memset(&self->fadeActive,0,0x10);
    self->fadeActive = '\x01';
  }
  else {
    memset(&self->fadeActive,0,0x10);
    self->fadeActive = '\x01';
    self->fadeLevel = 0.8;
    self->fadeStart = 0.8;
  }
  return;
}

