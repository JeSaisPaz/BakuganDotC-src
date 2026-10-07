// bdc 0x089a8df8 UiMainMenuStartModelFade
#include "bdc.h"

/* Prepares the alpha fade of the base model (`baseAlpha`) and the item models (`items[i].alpha`):
   0 -> 1 when opening, 1 -> 0 when closing. Stepped by `UiMainMenuStepModelFade`. */

void UiMainMenuStartModelFade(UiMainMenu *self, u8 closing)
{
  int i;

  if (closing == 0) {
    self->modelFadeT = 0.0f;
    self->baseAlpha = 0.0f;
    for (i = 0; i < 5; i++) {
      if (self->models[i] != (void *)0x0) {
        self->items[i].alpha = 0.0f;
      }
    }
  } else {
    self->modelFadeT = 0.0f;
    self->baseAlpha = 1.0f;
    for (i = 0; i < 5; i++) {
      if (self->models[i] != (void *)0x0) {
        self->items[i].alpha = 1.0f;
      }
    }
  }
}
