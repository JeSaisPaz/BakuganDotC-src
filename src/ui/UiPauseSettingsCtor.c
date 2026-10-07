// bdc 0x089ab824 UiPauseSettingsCtor
#include "bdc.h"

/* Constructor of UiPauseSettings, the in-game settings screen opened from the pause menu (menu
   result 0xe): three 0..10 slider values, an on/off toggle (`option_sol00`/`option_sol01`) and a
   return-to-title entry (`DWMesHelp` confirm, task 10020); task id 301 (0x12d), object size 0xbc0.
   Runs `UiScreenCtor`, installs `g_uiPauseSettingsVtbl`, runs the class init
   `UiPauseSettingsInit`, allocates (from the low heap) the 0x100-byte per-screen data block
   (`data`, +0x1c) and the zeroed 8-byte `g_uiSharedAnims` table, sets frame mode 0
   (`UiScreenSetFrameMode`), creates the fader system if needed (default fader sort key 20000),
   saves the pad's `stickEmulatesDpad` and turns it on, and keeps the shared background
   (`UiScreenKeepSharedBg`). Returns the screen. */

UiScreen *UiPauseSettingsCtor(UiPauseSettings *self)
{
    bool fromLow;
    void *block;
    GfxFab **anims;
    PadState *pad;

    UiScreenCtor(&self->base.base);
    self->base.base.vtable = g_uiPauseSettingsVtbl;
    UiPauseSettingsInit(self);

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(0x100, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->base.data = block;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    anims = (GfxFab **)MemAlloc(8, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    g_uiSharedAnims = anims;
    memset(anims, 0, 8);

    UiScreenSetFrameMode(&self->base.base, 0);
    self->unk6c = 0;
    if (!GfxFaderIsReady()) {
        GfxFaderSlotsInit(NULL);
        GfxGetActiveFader()->sortKey = 20000.0f;
    }
    pad = self->base.pad;
    self->unk70 = 0;
    self->savedStickEmulatesDpad = pad->stickEmulatesDpad;
    pad->stickEmulatesDpad = 1;
    UiScreenKeepSharedBg(&self->base);
    return &self->base;
}
