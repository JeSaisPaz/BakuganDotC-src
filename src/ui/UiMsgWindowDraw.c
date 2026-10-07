// bdc 0x08816a78 UiMsgWindowDraw
#include "bdc.h"

/* Draws the message window (`UiMsgWindowCtor`) when its alpha (`+0x8`) is non-zero: opens a render
   packet at depth `+0x4` (`GfxNewRenderPacket`), draws the object `+0x30` (`GfxRectDrawInPacket`) and
   the frame sprite layer `+0x38` (`GfxSpriteLayerDraw`). Called by the text render task
   (`UiTextTaskDraw`). */

void UiMsgWindowDraw(UiMsgWindow *self)
{
  void *packet;

  if (self->alpha != 0.0f) {
    packet = GfxNewRenderPacket(self->depth);
    if (self->rect != NULL) {
      GfxRectDrawInPacket(self->rect, packet);
    }
    if (self->spriteLayer != NULL) {
      GfxSpriteLayerDraw((GfxSpriteLayer *)self->spriteLayer, packet);
    }
  }
}
