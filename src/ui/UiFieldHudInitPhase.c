// bdc 0x088cf060 UiFieldHudInitPhase
#include "bdc.h"

/* Phase 0 of the field HUD: makes sure the UI text renderer exists (`UiTextRenderExists` /
   `UiTextRenderGetBox`), then goes to phase 1. */

void UiFieldHudInitPhase(UiFieldHud *self)

{
  float *box;

  if (!UiTextRenderExists()) {
    UiTextRenderEnsure();
    box = UiTextRenderGetBox();
    *box = 2000.0f;
  }
  (self->base).phase = 1;
  return;
}
