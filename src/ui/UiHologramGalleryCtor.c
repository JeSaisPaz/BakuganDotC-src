// bdc 0x0891c134 UiHologramGalleryCtor
#include "bdc.h"

/* Constructor of the hologram gallery screen, task id 391 (0x187), object size 0x22a8 (base
   `UiScreenCtor`, vtable `0x08af4a0c`): initialises its fields (`UiHologramGalleryInitState`:
   field/area from the battle id, the area's hologram slot layout, default Bakugan, menu locks, help
   order), allocates the 0x2f4-byte sprite table `data` from the low heap, sets frame mode 0, primes
   the fader (sort key 20000 when it has to be created), clears the display to opaque black and lets
   the stick emulate the d-pad. Returns `self`. The screen places holograms (`fix_ht_%02d_%02d`
   pictures, per-area slot layout) into the slots of a field area (`fix_f%01d_area_%02d`) for points
   (prices `UiHologramGalleryGetSlotCost`, saved in profile bytes `+0x84 + slot`), with
   `"fix_waku_01/02"` frames, brawler portraits `fix_bri_chara_%02d`, Bakugan names
   `f_cha_name_baku_%02d` and the help texts `"DWHologramHelp"`, and opens task 392
   (`UiHologramViewCtor`) as its sub-screen. */

UiScreen *UiHologramGalleryCtor(UiHologramGallery *self)
{
    bool fromLow;
    void *data;

    UiScreenCtor(&self->base.base);
    self->base.base.vtable = &g_uiHologramGalleryVtable;
    UiHologramGalleryInitState(self);
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    data = MemAlloc(189 * sizeof(GfxSprite *), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->base.data = data;
    UiScreenSetFrameMode(&self->base.base, 0);
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
    self->base.pad->stickEmulatesDpad = 1;
    return &self->base;
}
