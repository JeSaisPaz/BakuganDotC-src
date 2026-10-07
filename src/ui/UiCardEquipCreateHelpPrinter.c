// bdc 0x0896d294 UiCardEquipCreateHelpPrinter
#include "bdc.h"

/* Creates the card-help text printer `helpPrinter` of `UiCardEquip`: allocates
   0xf0 bytes from the low end of the heap and constructs it with `UiTextPrinterCtor` (font
   list `g_uiCardEquipHelpFontNames`) when the allocation succeeded, selects font 0, sets glyph
   scale 0.7, wrap width 120 (fewer than 3 Bakugan) or 140, and width scale 0x3e999999 (one ulp
   below 0.3). The pointer is stored and used without a NULL check. Then empties `helpText` and
   clears `helpDirty`, `helpAlpha` and `helpGlyphCount`. */
void UiCardEquipCreateHelpPrinter(UiCardEquip *self)
{
    bool fromLow;
    UiTextPrinter *printer;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(1);
    printer = MemAlloc(0xf0, 0, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (printer != NULL) {
        UiTextPrinterCtor(printer, 0, g_uiCardEquipHelpFontNames);
    }
    self->helpPrinter = printer;
    UiTextPrinterSetFont(printer, 0);
    self->helpPrinter->scale = 0.7f;
    if (self->bakuganCount < 3) {
        self->helpPrinter->wrapWidth = 120.0f;
    } else {
        self->helpPrinter->wrapWidth = 140.0f;
    }
    self->helpPrinter->widthScale = 0x1.333332p-2f; /* 0x3e999999, one ulp below 0.3f */
    strcpy(self->helpText, "");
    self->helpDirty = 0;
    self->helpAlpha = 0.0f;
    self->helpGlyphCount = 0.0f;
}
