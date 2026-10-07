// bdc 0x089a3f5c UiMainMenuScrollSea
#include "bdc.h"

/* Moves the UV scroll of the `psp_sea__DS2` material (`+0x9b0`) back by 1/600 per frame, wrapping
   to [0,1); hooked by `UiMainMenuHookSeaMaterial`. */

void UiMainMenuScrollSea(UiMainMenu *self)

{
  float scroll;

  scroll = self->seaScroll[0] - 0.0016666667f;
  self->seaScroll[0] = scroll;
  if (scroll < 0.0f) {
    scroll = scroll + 1.0f;
    self->seaScroll[0] = scroll;
  }
  if (!(scroll < 1.0f)) {
    self->seaScroll[0] = scroll - 1.0f;
  }
}
