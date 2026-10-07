// bdc 0x08817948 UiTextPrinterLoadFonts
#include "bdc.h"

/* Fills the text printer (`UiTextPrinterCtor`)'s 4-entry font texture table `fontTextures`
   (allocated on first use from the low heap) from the NULL-terminated name list `names` (default
   `g_uiDefaultFontNames` = `{"wd_font16", NULL}`) with `GfxFindTexture`. Returns 1 when all
   textures exist (or the list is empty), 0 at the first missing one. Called by
   `UiTextPrinterSetFont`. */

s32 UiTextPrinterLoadFonts(UiTextPrinter *self, const char **names)
{
  bool fromLow;
  void **textures;
  s32 i;

  if (self->fontTextures == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    textures = MemAlloc(4 * sizeof(void *), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->fontTextures = textures;
  }
  self->fontTextures[0] = NULL;
  self->fontTextures[1] = NULL;
  self->fontTextures[2] = NULL;
  self->fontTextures[3] = NULL;
  if (names == NULL) {
    names = g_uiDefaultFontNames;
  }
  for (i = 0; *names != NULL; i++) {
    self->fontTextures[i] = GfxFindTexture(*names);
    names++;
    if (self->fontTextures[i] == NULL) {
      return 0;
    }
  }
  return 1;
}
