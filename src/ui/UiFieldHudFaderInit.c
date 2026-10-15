// bdc 0x088c8e58 UiFieldHudFaderInit
#include "bdc.h"

/* Initialises the field HUD fader (`UiFieldHudFader` at `UiFieldHud+0x6c`): stores the HUD sprite
   table `*sprites`, alpha 1, state 0 (idle). Called by `UiFieldHudSetupPhase`. */

void UiFieldHudFaderInit(void **fader, void **sprites)

{
  UiFieldHudFader *p;

  p = *fader;
  p->sprites = *sprites;
  p->alpha = 1.0f;
  p->state = 0;
  return;
}
