// bdc 0x0898b24c UiCollectionFigureCtor
#include "bdc.h"

/* Constructor of UiCollectionFigure, the collection figure viewer (`figure_refmap`,
   `*_U_figure.gmo`); task id 314 (0x13a). Runs `UiScreenCtor`, installs
   `g_uiCollectionFigureVtbl`, stores `category` (+0xe7d), runs the class init
   `UiCollectionFigureInitState`, allocates the 0x114-byte per-screen sprite table (`data`,
   +0x1c) from the low heap, sets frame mode 0 (`UiScreenSetFrameMode`), creates the fader system
   if needed (sort key 20000), clears the clear colour to opaque black, makes the stick emulate the
   d-pad, sets the shared-background hand-over flag `g_uiKeepSharedBg` and puts texture
   `figure_refmap` into texture slot B 0 (`GfxSetTextureSlotB`). Returns the screen. */

UiScreen *UiCollectionFigureCtor(UiCollectionFigure *self, s8 category)
{
  bool fromLow;
  void *data;
  GfxFader *fader;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiCollectionFigureVtbl;
  self->category = category;
  UiCollectionFigureInitState(self);
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(0x114, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->base.data = data;
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
  self->base.pad->stickEmulatesDpad = 1;
  g_uiKeepSharedBg = 1;
  GfxSetTextureSlotB(GfxFindTexture("figure_refmap"), 0);
  return &self->base;
}
