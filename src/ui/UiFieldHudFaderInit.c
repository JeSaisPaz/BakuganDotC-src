// bdc 0x088c8e58 UiFieldHudFaderInit
#include "bdc.h"

/* Initialises the field HUD fader (`UiFieldHud+0x6c`, pointer to a 0xc-byte `{s32 state, sprites,
   f32 alpha}`): stores the HUD sprite table `*sprites`, alpha 1, state 0 (idle). Called by
   `UiFieldHudSetupPhase`. */

void UiFieldHudFaderInit(void **fader, void **sprites)

{
  void **p;

  p = *fader;
  p[1] = *sprites;
  ((float *)p)[2] = 1.0f;
  p[0] = (void *)0;
  return;
}
