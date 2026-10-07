// bdc 0x089febd8 UiWindowFrameSetStyle
#include "bdc.h"

/* Re-skins a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `0x08af5954` at `+0x74`, plus a `CoreObject`
   at `+0x80`) with style `style` of the current style set (`g_uiFrameStyleRoot`)
   (`UiWindowFrameApplyStyle`) (vtable `+0x44`). */

void UiWindowFrameSetStyle(UiWindowFrame *self, int style)

{
  UiWindowFrameApplyStyle(self, (char *)&g_uiFrameStyleRoot->styles[style], style);
  return;
}

