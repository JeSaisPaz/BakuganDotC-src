// bdc 0x08928fd0 UiHologramViewCtor
#include "bdc.h"

/* Constructor of the hologram view screen, task id 392 (0x188), object size 0x710 (base
   `UiScreenCtor`, vtable `g_uiHologramViewVtable`), built by `CoreTaskNewByIdArg` with a
   one-byte argument (the view kind `+0x485`): initialises its fields (`UiHologramViewInitState`),
   allocates the 0x68-byte screen data block from the low heap, sets frame mode 0, clears `unk6c`/
   `unk70`, creates the fader system if needed (active fader sort key 20000), sets the clear colour
   to opaque black, creates the text printer (`UiHologramViewCreateTextPrinter`), makes the stick
   emulate the d-pad and sets `g_uiKeepSharedBg` to 1. Returns the screen. Opened by
   `UiHologramGalleryOpenViewPhase` (task 391); shows a Bakugan model (`"*_stay_sel"`/
   `"*_stay_sel_turn"` motions on the `"menu_daiza.gmo"` pedestal), its card (`"card_SS_%03d"`),
   the `"fix_tyu_%02d"` page pictures and `"tips_kao%01d"` faces, and narrated `"DWHologramHelp"`
   pages (`UiHologramViewRunPages`, `UiHologramViewPlayVoice`). */

UiScreen *UiHologramViewCtor(UiHologramView *self, u32 arg)
{
    bool fromLow;
    void *data;

    UiScreenCtor((CoreTask *)self);
    self->base.base.vtable = g_uiHologramViewVtable;
    self->kind = (u8)arg;
    UiHologramViewInitState(self);
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    data = MemAlloc(0x68, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->base.data = data;
    UiScreenSetFrameMode((CoreTask *)self, 0);
    self->unk6c = 0;
    if (!GfxFaderIsReady()) {
        GfxFaderSlotsInit(NULL);
        GfxGetActiveFader()->sortKey = 20000.0f;
    }
    self->unk70 = 0;
    g_gfxDisplay->clearColor[0] = 0.0f;
    g_gfxDisplay->clearColor[1] = 0.0f;
    g_gfxDisplay->clearColor[2] = 0.0f;
    g_gfxDisplay->clearColor[3] = 1.0f;
    UiHologramViewCreateTextPrinter(self);
    self->base.pad->stickEmulatesDpad = 1;
    g_uiKeepSharedBg = 1;
    return &self->base;
}
