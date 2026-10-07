// bdc 0x0894da44 UiNetMenuCreateHelpText
#include "bdc.h"

/* Creates the help-text printer of `UiNetMenu`: a 0xf0-byte
   `UiTextPrinter` from low memory at `+0x2dc` (font 1, scale 0.8, wrap width
   1000, spacing ~0.53), empties `+0x308` and resets its alpha state (`+0x2e0` = 0, `+0x2e4` = 1,
   glyph count `+0x2ec` = 0). */

void UiNetMenuCreateHelpText(UiScreen *screen)

{
  UiNetMenu *menu = (UiNetMenu *)screen;
  bool fromLow;
  UiTextPrinter *self;
  UiTextPrinter *printer;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  self = MemAlloc(sizeof(UiTextPrinter), (char *)0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  printer = (UiTextPrinter *)0;
  if (self != (UiTextPrinter *)0) {
    UiTextPrinterCtor(self, 0, (char **)0);
    printer = self;
  }
  menu->helpPrinter = (GfxSprite *)printer;
  UiTextPrinterSetFont(printer, 1);
  ((UiTextPrinter *)menu->helpPrinter)->scale = 0.8f;
  ((UiTextPrinter *)menu->helpPrinter)->wrapWidth = 1000.0f;
  ((UiTextPrinter *)menu->helpPrinter)->widthScale = 0.53333336f;
  strcpy((char *)menu->helpText, g_uiNetMenuEmptyText);
  menu->titleHelpAlpha = 0.0f;
  menu->helpAppliedAlpha = 1.0f;
  menu->helpGlyphCount = 0.0f;
}
