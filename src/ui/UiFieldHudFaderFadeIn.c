// bdc 0x088c8e88 UiFieldHudFaderFadeIn
#include "bdc.h"

/* Starts fading the field HUD fader (`UiFieldHud+0x6c`, pointer to a 0xc-byte `{s32 state, sprites,
   f32 alpha}`) back in (state 2). Caller: `GameFieldCameraExitMode9`. */

void UiFieldHudFaderFadeIn(void **fader)

{
  *(s32 *)*fader = 2;
  return;
}

