// bdc 0x088c8e78 UiFieldHudFaderFadeOut
#include "bdc.h"

/* Starts fading the field HUD fader (`UiFieldHud+0x6c`, pointer to a 0xc-byte `{s32 state, sprites,
   f32 alpha}`) out (state 1). Caller: `ActorPlayerStateHoldRFocusPoint` (with
   `GameFieldCameraBeginMode9`). */

void UiFieldHudFaderFadeOut(void **fader)

{
  *(s32 *)*fader = 1;
  return;
}

