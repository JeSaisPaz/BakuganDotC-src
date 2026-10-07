// bdc 0x089febd8 UiWindowFrameSetStyle
#include "bdc.h"

/* Re-skins a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `0x08af5954` at `+0x74`, plus a `CoreObject`
   at `+0x80`) with style `style` from the table at `*(0x08ac6218) + 4`
   (`UiWindowFrameApplyStyle`) (vtable `+0x44`). */

void UiWindowFrameSetStyle(UiWindowFrame *self, int style)

{
  UiWindowFrameApplyStyle(self,(g_uiFrameStyleRoot[1] + style * 0xa0),style);
  return;
}

