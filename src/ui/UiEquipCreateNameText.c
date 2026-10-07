// bdc 0x08959a78 UiEquipCreateNameText
#include "bdc.h"

/* Creates the name text printer of `UiEquip`: 0xf0-byte
   `UiTextPrinter` at `+0x5028` (font 3, scale 0.6, wrap 1000, spacing ~0.53),
   empties `+0x5030`, resets its alpha state. */

void UiEquipCreateNameText(UiEquip *self)

{
  bool fromLow;
  UiTextPrinter *mem;
  UiTextPrinter *printer;
  
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0xf0,NULL,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  printer = NULL;
  if (mem != NULL) {
    UiTextPrinterCtor(mem,0,NULL);
    printer = mem;
  }
  self->namePrinter = printer;
  UiTextPrinterSetFont(printer,3);
  self->namePrinter->scale = 0.6f;
  self->namePrinter->wrapWidth = 1000.0f;
  self->namePrinter->widthScale = 0.53333336f;
  strcpy(self->nameText,"");
  self->nameDirty = 0;
  self->nameAlpha = 0.0f;
  self->nameFirstGlyph = 0.0f;
  self->nameGlyphCount = 0.0f;
}
