// bdc 0x089f3e88 GfxSpriteSetQuadMode
#include "bdc.h"

/* Switches a sprite's quad template (`quadMode`) to `mode` when it differs. Mode 0 points
   `vertices` at `g_gfxSpriteDefaultQuad`, mode 1 at `g_gfxQuadTemplateMode1`. Modes 2, 3 and 4
   copy a 4-vertex template into `vertexBuf` and point `vertices` at it: 2 uses
   `g_gfxQuadTemplateTopLeft`; 3 uses `g_gfxQuadTemplateCentreFlipped` when the old `vertices`
   were `g_gfxQuadTemplate`, else `g_gfxQuadTemplateCentre`; 4 uses
   `g_gfxQuadTemplateBottomCentreFlipped` when `billboardMode == 4`, else
   `g_gfxQuadTemplateBottomCentre`. Modes >= 5 only store the value. Same mode: no change. */

static void GfxSpriteCopyQuadTemplate(GfxSprite *sprite, const GfxSpriteVertex *tmpl)
{
  int i;

  /* the asm copies the 48 bytes as three lv.q/sv.q quads */
  for (i = 0; i < 4; i++) {
    sprite->vertexBuf[i] = tmpl[i];
  }
}

void GfxSpriteSetQuadMode(GfxSprite *sprite, u32 mode)
{
  if (sprite->quadMode == mode) {
    return;
  }
  if (mode < 5) {
    if (mode == 1) {
      sprite->vertices = (GfxSpriteVertex *)g_gfxQuadTemplateMode1;
    }
    else if (mode == 2) {
      GfxSpriteCopyQuadTemplate(sprite, g_gfxQuadTemplateTopLeft);
      sprite->vertices = sprite->vertexBuf;
    }
    else if (mode == 3) {
      if (sprite->vertices == (GfxSpriteVertex *)&g_gfxQuadTemplate) {
        GfxSpriteCopyQuadTemplate(sprite, g_gfxQuadTemplateCentreFlipped);
      }
      else {
        GfxSpriteCopyQuadTemplate(sprite, g_gfxQuadTemplateCentre);
      }
      sprite->vertices = sprite->vertexBuf;
    }
    else if (mode == 4) {
      if (sprite->billboardMode == 4) {
        GfxSpriteCopyQuadTemplate(sprite, g_gfxQuadTemplateBottomCentreFlipped);
      }
      else {
        GfxSpriteCopyQuadTemplate(sprite, g_gfxQuadTemplateBottomCentre);
      }
      sprite->vertices = sprite->vertexBuf;
    }
    else {
      sprite->vertices = (GfxSpriteVertex *)g_gfxSpriteDefaultQuad;
    }
  }
  sprite->quadMode = mode;
}
