// bdc 0x08973604 UiCollectionMenuCtor
#include "bdc.h"

/* Constructor of UiCollectionMenu, the collection top menu (`Card_light`, `Figure_light`,
   `Sphere_light`, `Theater_light`); task id 311 (0x137), object size 0x764. Runs `UiScreenCtor`,
   installs `g_uiCollectionMenuVtbl`, runs the class init `UiCollectionMenuInitState`, allocates
   the 0x68-byte per-screen sprite table (`data`, +0x1c) and the zeroed two-slot table
   `g_uiSharedAnims` from the low heap, sets frame mode 0 (`UiScreenSetFrameMode`), creates the
   fader system if needed (sort key 20000), clears the clear colour to opaque black, saves the pad's
   `stickEmulatesDpad` in `savedStickEmulatesDpad` and sets it, then keeps the shared background
   (`UiScreenKeepSharedBg`). Returns the screen. */

UiScreen *UiCollectionMenuCtor(UiCollectionMenu *self)
{
  bool fromLow;
  void *data;
  GfxFader *fader;
  PadState *pad;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiCollectionMenuVtbl;
  UiCollectionMenuInitState(self);
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(0x68, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->base.data = data;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(8, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_uiSharedAnims = (GfxFab **)data;
  memset(data, 0, 8);
  UiScreenSetFrameMode((CoreTask *)self, 0);
  self->unk6c = 0;
  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit(NULL);
    fader = GfxGetActiveFader();
    fader->sortKey = 20000.0f;
  }
  self->unk70 = 0;
  g_gfxDisplay->clearColor[0] = 0.0f;
  g_gfxDisplay->clearColor[1] = 0.0f;
  g_gfxDisplay->clearColor[2] = 0.0f;
  g_gfxDisplay->clearColor[3] = 1.0f;
  pad = self->base.pad;
  self->savedStickEmulatesDpad = pad->stickEmulatesDpad;
  pad->stickEmulatesDpad = 1;
  UiScreenKeepSharedBg(&self->base);
  return &self->base;
}
