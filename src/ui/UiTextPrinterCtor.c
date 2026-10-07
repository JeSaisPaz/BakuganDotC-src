// bdc 0x088176c8 UiTextPrinterCtor
#include "bdc.h"

/* Constructor of the text printer (0xf0 bytes, 2D sprite-layer base GfxSpriteLayerCtor, printer
   vtable `g_uiTextPrinterVtbl` at `+0x74`): allocates the 4-entry font texture table `+0x80` from
   the low heap and fills it with the textures named by the NULL-terminated list `fontNames`
   (default `g_uiDefaultFontNames` = {"wd_font16", NULL}, `GfxFindTexture`); a non-NULL `texList`
   is swapped in as the texture list for the lookups and the previous list restored afterwards.
   `glyphsPerRow` = CLUT colours of the first texture / 16; glyph advance/line height 17, cell 16x16,
   scale 1, text and outline colour `g_colorWhite`, wrap width 10000, max lines -1.
   Returns `self`. */

UiTextPrinter *UiTextPrinterCtor(UiTextPrinter *self, void *texList, char **fontNames)
{
  bool fromLow;
  void **textures;
  void *prevList;
  int i;

  GfxSpriteLayerCtor(&self->layer, 0);
  self->layer.vtbl = &g_uiTextPrinterVtbl;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  textures = MemAlloc(0x10, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->fontTextures = textures;
  if (fontNames == NULL) {
    fontNames = (char **)g_uiDefaultFontNames;
  }
  if (texList != NULL) {
    prevList = GfxTextureListSwap(texList);
    for (i = 0; *fontNames != NULL; i++) {
      self->fontTextures[i] = GfxFindTexture(*fontNames);
      fontNames++;
    }
    GfxTextureListSwap(prevList);
  } else {
    for (i = 0; *fontNames != NULL; i++) {
      self->fontTextures[i] = GfxFindTexture(*fontNames);
      fontNames++;
    }
  }
  self->sjisFlag = 0;
  self->glyphsPerRow = GfxTextureGetClutColors(self->fontTextures[0]) / 16;
  self->advanceX = 17.0f;
  self->lineHeight = 17.0f;
  self->spacing = 0.0f;
  self->cellW = 16.0f;
  self->cellH = 16.0f;
  self->scale = 1.0f;
  self->color[0] = g_colorWhite.x;
  self->color[1] = g_colorWhite.y;
  self->color[2] = g_colorWhite.z;
  self->color[3] = g_colorWhite.w;
  self->outlineColor[0] = g_colorWhite.x;
  self->outlineColor[1] = g_colorWhite.y;
  self->outlineColor[2] = g_colorWhite.z;
  self->outlineColor[3] = g_colorWhite.w;
  self->ctrlF0Flag = 0;
  self->inInsert = 0;
  self->insertText = NULL;
  self->widthTable = NULL;
  self->widthScale = 1.0f;
  self->wrapWidth = 10000.0f;
  self->glyphs = NULL;
  self->glyphCount = 0;
  self->maxLines = -1;
  self->resumePos = 0;
  return self;
}
