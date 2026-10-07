// bdc 0x089fe858 UiWindowFrameNew
#include "bdc.h"

/* Allocates and builds a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `0x08af5954` at `+0x74`, plus a `CoreObject`
   at `+0x80`): `UiWindowFrameCtor` on 0x130 low-heap bytes, user word `+0xa0` = `arg`, depth
   `+0xf0` = `depth`, position vec4 `+0xb0` = `pos`, builds the sprites from `style`
   (`UiWindowFrameBuild`) and runs the layout virtual. Returns the frame. */

void *UiWindowFrameNew(char *style, const float *rect, u32 arg, float depth)

{
  bool fromLow;
  UiWindowFrame *self;
  UiWindowFrame *frame;
  const VtblEntry *layout;

  frame = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  self = MemAlloc(0x130, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (self != NULL) {
    UiWindowFrameCtor(self);
    frame = self;
  }
  *(u32 *)frame->userWord = arg;
  frame->depth = depth;
  frame->rect[0] = rect[0];
  frame->rect[1] = rect[1];
  frame->rect[2] = rect[2];
  frame->rect[3] = rect[3];
  UiWindowFrameBuild(frame, style);
  layout = &((GfxSpriteLayer *)frame)->vtbl[7];
  ((void (*)(void *))layout->fn)((u8 *)frame + layout->delta);
  return frame;
}
