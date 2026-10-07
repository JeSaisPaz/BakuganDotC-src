// bdc 0x0895002c UiTitleMenuCtor
#include "bdc.h"

/* Constructor of the title/system menu screen, task id 1000 (0x3e8) (base `UiScreenCtor`, vtable
   `0x08af4db4`). Object size 0x74. Allocates two 8-byte blocks from the low heap (`data`, and
   `bgData` zeroed), sets frame mode 1, lazily creates the fader system (active fader sort key
   20000) and the shared text box (packet depth 2000), sets the clear colour to opaque black,
   lets the stick emulate the d-pad, and returns the screen. */

UiScreen *UiTitleMenuCtor(UiScreen *screen)
{
  UiTitleMenu *self = (UiTitleMenu *)screen;
  bool fromLow;
  void *block;
  GfxFader *fader;
  float *box;

  UiScreenCtor(&screen->base);
  screen->base.vtable = &g_uiTitleMenuVtable;
  CoreRandLoadVfpuState();

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(2 * sizeof(GfxSprite *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  screen->data = block;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(2 * sizeof(GfxFab *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  screen->bgData = block;
  memset(block, 0, 2 * sizeof(GfxFab *));

  UiScreenSetFrameMode(&screen->base, 1);
  self->package = NULL;
  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit(NULL);
    fader = GfxGetActiveFader();
    fader->sortKey = 20000.0f;
  }
  if (!UiTextRenderExists()) {
    UiTextRenderEnsure();
    box = UiTextRenderGetBox();
    *box = 2000.0f;
  }
  self->unk70 = 0;
  g_gfxDisplay->clearColor[0] = 0.0f;
  g_gfxDisplay->clearColor[1] = 0.0f;
  g_gfxDisplay->clearColor[2] = 0.0f;
  g_gfxDisplay->clearColor[3] = 1.0f;
  screen->unk30 = 0;
  screen->pad->stickEmulatesDpad = 1;
  return screen;
}
