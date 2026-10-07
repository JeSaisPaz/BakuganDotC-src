// bdc 0x089100c4 UiPauseCtor
#include "bdc.h"

/* Constructor of the pause menu, task id 410 (0x19a) (base `UiScreenCtor`, vtable `g_uiPauseVtbl`).
   Sets `spriteCount` to 0x25 and allocates the 0x94-byte sprite table (`data`) from the low heap,
   selects frame mode 1, saves the pad's `stickEmulatesDpad` byte and forces it on, refreshes
   `g_profileFlag0` (`SaveRefreshProfileFlag0`), clears the menu fields (scale 1.0/0.0, no
   confirm message), creates the two hint-line printers (font 1, wrap width 460, empty text; the
   first gets a red outline `g_colorRed`), sets 2 entries and kind 0 and creates the shared help
   line (`UiHelpLineCreate`). Returns `&self->base`. */

UiScreen *UiPauseCtor(UiPause *self)
{
    bool fromLow;
    UiTextPrinter *printer;
    PadState *pad;
    int i;

    UiScreenCtor((CoreTask *)self);
    self->base.base.vtable = &g_uiPauseVtbl;
    self->spriteCount = 0x25;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    self->base.data = MemAlloc(0x25 * sizeof(GfxSprite *), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    UiScreenSetFrameMode((CoreTask *)self, 1);
    pad = self->base.pad;
    self->savedStickEmulatesDpad = pad->stickEmulatesDpad;
    pad->stickEmulatesDpad = 1;
    SaveRefreshProfileFlag0();
    self->base.unk24 = 0;
    self->unk70 = 0;
    self->cursor = 0;
    self->unk140 = 0.0f;
    self->unk144 = 0.0f;
    memset(self->unk80, 0, sizeof(self->unk80));
    self->leaving = 0;
    self->unk158 = 0;
    self->scaleY = 0.0f;
    self->scaleX = 1.0f;
    self->cursorPulse = 0.0f;
    self->hintAlpha = 0.0f;
    self->confirmMsg = -1;
    for (i = 0; i < 2; i++) {
        printer = UiTextPrinterCreate(1);
        self->hintPrinters[i] = printer;
        printer->wrapWidth = 460.0f;
        strcpy(self->hintText[i], "");
    }
    printer = self->hintPrinters[0];
    printer->outlineColor[0] = g_colorRed.x;
    printer->outlineColor[1] = g_colorRed.y;
    printer->outlineColor[2] = g_colorRed.z;
    printer->outlineColor[3] = g_colorRed.w;
    self->entryCount = 2;
    self->unk164 = 0;
    self->kind = 0;
    UiHelpLineCreate();
    return &self->base;
}
