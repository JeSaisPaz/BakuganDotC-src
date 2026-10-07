// bdc 0x089a468c UiHelpLineDraw
#include "bdc.h"

/* Draws the shared one-line help text (printer g_helpLinePrinter, see `UiHelpLineCreate`): applies
   the pending alpha g_helpLineAlpha to every glyph sprite when it changed, sets the outline colour
   from g_colorWhite and submits the text layer in a render packet at depth 5000. */

void UiHelpLineDraw(void)

{
  void *packet;
  int i;
  GfxSprite *sprite;
  float *dst;

  if ((g_helpLineGlyphCount != 0.0f) && (g_helpLineAppliedAlpha != g_helpLineAlpha)) {
    i = 0;
    sprite = g_helpLineGlyphHead;
    if (0.0f < g_helpLineGlyphCount) {
      do {
        i = i + 1;
        sprite->alpha = g_helpLineAlpha;
        sprite = sprite->next;
      } while ((float)i < g_helpLineGlyphCount);
    }
    g_helpLineAppliedAlpha = g_helpLineAlpha;
  }
  /* outlineColor = g_colorWhite (lv.q/sv.q quad copy) */
  dst = g_helpLinePrinter->outlineColor;
  dst[0] = g_colorWhite.x;
  dst[1] = g_colorWhite.y;
  dst[2] = g_colorWhite.z;
  dst[3] = g_colorWhite.w;
  packet = GfxNewRenderPacket(5000.0f);
  GfxSpriteLayerDraw(&g_helpLinePrinter->layer, packet);
}
