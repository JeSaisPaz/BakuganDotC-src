// bdc 0x088cb850 UiTalkBalloonDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the talk balloon (`UiTalkBalloonCtor`): skipped while task 100 has flag
   2; opens a render packet at depth `g_talkBalloonDepthBase + +0x74`, draws the choice highlight quads
   (`UiTalkBalloonDrawHighlights`), the balloon frame (`UiTalkBalloonDrawFrame`), the text
   printer layer `+0x10` and, one level below, the extra sprite layer `+0x208`. */

void UiTalkBalloonDraw(UiTalkBalloon *self)

{
  void *task;
  void *packet;
  float depth;

  if (self->printer != NULL) {
    task = CoreTaskFind(100);
    if (task != NULL && CoreTaskHasFlags(task, 2)) {
      return;
    }
    depth = g_talkBalloonDepthBase + self->depth;
    packet = GfxNewRenderPacket(depth);
    UiTalkBalloonDrawHighlights(self, packet);
    UiTalkBalloonDrawFrame(self, packet);
    GfxSpriteLayerDraw(&self->printer->layer, packet);
    if (self->partLayer != NULL) {
      packet = GfxNewRenderPacket(depth - 1.0f);
      GfxSpriteLayerDraw(self->partLayer, packet);
    }
  }
}
