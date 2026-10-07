// bdc 0x089b428c UiComboListClose
#include "bdc.h"

/* Closes `UiComboList`: plays sound 7 (`SndManagerPlay` when the sound manager
   exists), sets the menu result `result` (`UiSetMenuResult`) and advances to the next phase. */

void UiComboListClose(UiComboList *self, s32 result)

{
  SndManager *mgr;
  
  if (SndHasManager()) {
    mgr = SndGetManager();
    SndManagerPlay(mgr,7,0,0);
  }
  UiSetMenuResult(&self->base,result);
  (self->base).phase = (self->base).phase + 1;
  return;
}

