// bdc 0x089fe810 UiWindowFrameCreate
#include "bdc.h"

/* Creates a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `0x08af5954` at `+0x74`, plus a `CoreObject`
   at `+0x80`) using style `style` of the current style set (`g_uiFrameStyleRoot``->styles`,
   0xa0-byte records); copies `rect` (position vec4) and forwards it with `depth` and the style
   index (user word) to `UiWindowFrameNew`. */

void *UiWindowFrameCreate(int style, const float *rect, float depth)

{
  float pos[4];

  pos[0] = rect[0];
  pos[1] = rect[1];
  pos[2] = rect[2];
  pos[3] = rect[3];
  return UiWindowFrameNew((char *)&g_uiFrameStyleRoot->styles[style], pos, style, depth);
}
