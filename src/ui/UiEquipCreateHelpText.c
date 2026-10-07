// bdc 0x08959b70 UiEquipCreateHelpText
#include "bdc.h"

/* Creates the help text printer of `UiEquip`: allocates 0xf0 bytes from the low end
   of the heap and builds a `UiTextPrinter` with `g_uiEquipHelpFontNames`
   into `helpPrinter` (+0x502c; NULL if the allocation failed, and font 1 is still selected on it),
   then sets scale 0.9, wrap width 160 and width scale ~0.567, empties `helpText` (+0x5050) and
   clears `helpDirty`, `helpAlpha`, `helpFirstGlyph` and `helpGlyphCount`. */

void UiEquipCreateHelpText(UiEquip *self)
{
  bool fromLow;
  UiTextPrinter *mem;
  UiTextPrinter *printer;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(UiTextPrinter), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  printer = NULL;
  if (mem != NULL) {
    UiTextPrinterCtor(mem, 0, g_uiEquipHelpFontNames);
    printer = mem;
  }
  self->helpPrinter = printer;
  UiTextPrinterSetFont(printer, 1);
  self->helpPrinter->scale = 0.9f;
  /* Both branches of the original (playerCount < 3 or not) store the same 160. */
  self->helpPrinter->wrapWidth = 160.0f;
  self->helpPrinter->widthScale = 0.56666666f;
  strcpy(self->helpText, "");
  self->helpDirty = 0;
  self->helpAlpha = 0.0f;
  self->helpFirstGlyph = 0.0f;
  self->helpGlyphCount = 0.0f;
}
