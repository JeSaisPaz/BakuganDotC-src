// bdc 0x089791c8 UiCollectionSphereCtor
#include "bdc.h"

/* Constructor of UiCollectionSphere, the collection sphere viewer (`cha_spherename_colle_%02d`,
   `*_dir_popout` models); task id 312 (0x138). Runs `UiScreenCtor`, installs
   `g_uiCollectionSphereVtbl`, stores `category` (+0xee5), runs the class init
   `UiCollectionSphereInitState`, allocates the 0x11c-byte per-screen sprite table (`data`,
   +0x1c) from the low heap, sets frame mode 0 (`UiScreenSetFrameMode`), creates the fader system
   if needed (sort key 20000), clears the clear colour to opaque black, makes the stick emulate the
   d-pad, sets the shared-background hand-over flag `g_uiKeepSharedBg` and puts texture
   `tex_ref00` into texture slot B 0 (`GfxSetTextureSlotB`). Returns the screen. */

UiScreen *UiCollectionSphereCtor(UiCollectionSphere *self, s8 category)
{
  bool fromLow;
  void *data;
  GfxFader *fader;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiCollectionSphereVtbl;
  self->category = category;
  UiCollectionSphereInitState(self);
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(0x11c, NULL, 0);
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
  GfxSetTextureSlotB(GfxFindTexture("tex_ref00"), 0);
  return &self->base;
}
