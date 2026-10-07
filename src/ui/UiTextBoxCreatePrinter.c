// bdc 0x089eb00c UiTextBoxCreatePrinter
#include "bdc.h"

/* Creates the text box's printer once the font is available (`UiTextFontReady`): allocates a
   0xf0-byte text printer with the `wd_font16` font (`UiTextPrinterCtor`), stores it at `+0xc`,
   optionally sizes its buffer to `maxChars` (`GfxSpriteLayerInitPool`, when > 0) and gives it the
   box's depth (`+0x4`, or derives it with `UiTextBoxSetDepth` when 0). Returns 1 on success, 0
   while the font is not ready (callers poll it every frame). */

s32 UiTextBoxCreatePrinter(UiTextBox *box, s32 maxChars)

{
  bool fromLow;
  UiTextPrinter *self;
  UiTextPrinter *self_00;
  s32 ok;
  
  ok = 0;
  if (UiTextFontReady()) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    self = MemAlloc(0xf0,(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self_00 = (UiTextPrinter *)0x0;
    if (self != (UiTextPrinter *)0x0) {
      UiTextPrinterCtor(self,0,(char **)&g_uiTextFontName);
      self_00 = self;
    }
    box->printer = self_00;
    if (0 < maxChars) {
      GfxSpriteLayerInitPool(&self_00->layer,maxChars);
      self_00 = box->printer;
    }
    if (box->depth == 0.0) {
      UiTextBoxSetDepth(self_00->advanceX,box);
      ok = 1;
    }
    else {
      self_00->advanceX = box->depth;
      ok = 1;
    }
  }
  return ok;
}

