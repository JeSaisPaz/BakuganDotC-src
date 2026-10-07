// bdc 0x089b42f8 UiComboListMainPhase
#include "bdc.h"

/* Phase 2 of the combo list screen: scroll/page input (`UiComboListScrollBg`); the cancel button (pad bit
   0x20 of byte 5) starts closing (`UiComboListClose(screen, 0)`). */

void UiComboListMainPhase(UiComboList *self)

{
  UiComboListScrollBg(self);
  if ((((self->base).pad)->pressed & 0x2000) != 0) {
    UiComboListClose(self,0);
  }
  return;
}

