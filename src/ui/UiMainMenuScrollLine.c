// bdc 0x089a3f04 UiMainMenuScrollLine
#include "bdc.h"

/* Advances the UV scroll of the `psp_line__BA` material (`+0x9a0`) by 1/600 per frame, wrapping to
   [0,1). Driven from `UiMainMenuUpdateModels`; the material hook is
   `UiMainMenuHookLineMaterial`. */

void UiMainMenuScrollLine(UiMainMenu *self)

{
  float scroll;
  
  scroll = self->lineScroll[0] + 0.0016666667f;
  self->lineScroll[0] = scroll;
  if (scroll < 0.0f) {
    scroll = scroll + 1.0f;
    self->lineScroll[0] = scroll;
  }
  if (!(scroll < 1.0f)) {
    self->lineScroll[0] = scroll - 1.0f;
  }
  return;
}

