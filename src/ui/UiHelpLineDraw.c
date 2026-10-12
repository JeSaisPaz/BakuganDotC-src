// bdc 0x089a468c UiHelpLineDraw
#include "bdc.h"

/* Draws the shared one-line help text (`g_helpLine`, see `UiHelpLineCreate`): applies the
   pending alpha to every glyph sprite when it changed, sets the outline colour from g_colorWhite
   and submits the text layer in a render packet at depth 5000. */

void UiHelpLineDraw(void)

{
  void *packet;
  int i;
  GfxSprite *sprite;
  float *dst;

  if ((g_helpLine.glyphCount != 0.0f) && (g_helpLine.appliedAlpha != g_helpLine.alpha)) {
    i = 0;
    sprite = g_helpLine.glyphHead;
    if (0.0f < g_helpLine.glyphCount) {
      do {
        i = i + 1;
        sprite->alpha = g_helpLine.alpha;
        sprite = sprite->next;
      } while ((float)i < g_helpLine.glyphCount);
    }
    g_helpLine.appliedAlpha = g_helpLine.alpha;
  }
  /* outlineColor = g_colorWhite (lv.q/sv.q quad copy) */
  dst = g_helpLine.printer->outlineColor;
  dst[0] = g_colorWhite.x;
  dst[1] = g_colorWhite.y;
  dst[2] = g_colorWhite.z;
  dst[3] = g_colorWhite.w;
  packet = GfxNewRenderPacket(5000.0f);
  GfxSpriteLayerDraw(&g_helpLine.printer->layer, packet);
}
