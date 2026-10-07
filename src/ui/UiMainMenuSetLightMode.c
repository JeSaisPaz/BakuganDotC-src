// bdc 0x089a6040 UiMainMenuSetLightMode
#include "bdc.h"

/* Resets the base-model light animation state (`+0xe30`, 12 bytes) and sets its mode byte to `mode`
   (see `UiMainMenuUpdateItemBoxLights`). */

void UiMainMenuSetLightMode(UiMainMenu *self, u8 mode)

{
  memset(&self->lightMode,0,0xc);
  self->lightMode = mode;
  return;
}

