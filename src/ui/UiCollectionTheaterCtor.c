// bdc 0x08987b34 UiCollectionTheaterCtor
#include "bdc.h"

/* Constructor of UiCollectionTheater, the collection theater (`collection_scene_%02d`,
   `cinema_%02d`); task id 315 (0x13b), object size 0x964. Runs `UiScreenCtor`, installs vtable
   `g_uiCollectionTheaterVtbl`, runs the class init `UiCollectionTheaterInitState`, allocates
   the 0xc4-byte per-screen data block (`data`, +0x1c) from the low heap, sets frame mode 0
   (`UiScreenSetFrameMode`), zeroes `unk6c`/`unk70`, creates the fader if needed (sort key
   20000), clears the clear colour to opaque black, makes the stick emulate the d-pad and sets the
   shared-background hand-over flag `g_uiKeepSharedBg`. Returns `screen`. */

UiScreen *UiCollectionTheaterCtor(UiScreen *screen)

{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  bool fromLow;
  void *data;

  UiScreenCtor(&self->base.base);
  self->base.base.vtable = g_uiCollectionTheaterVtbl;
  UiCollectionTheaterInitState(&self->base);
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(49 * sizeof(GfxSprite *), NULL, 0);
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
  g_uiKeepSharedBg = 1;
  return screen;
}
