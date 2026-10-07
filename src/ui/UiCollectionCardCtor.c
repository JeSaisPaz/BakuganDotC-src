// bdc 0x08982508 UiCollectionCardCtor
#include "bdc.h"

/* Constructor of UiCollectionCard, the collection card viewer (`collection_ability_%02d`,
   `card_L_%03d`); task id 313 (0x139). Runs `UiScreenCtor`, installs
   `g_uiCollectionCardVtbl`, stores `category` (+0xbd1), runs the class init
   `UiCollectionCardInitState`, allocates the 0x108-byte per-screen sprite table (`data`, +0x1c)
   from the low heap, sets frame mode 0 (`UiScreenSetFrameMode`), creates the fader system if
   needed (sort key 20000), clears the clear colour to opaque black, makes the stick emulate the
   d-pad and sets the shared-background hand-over flag `g_uiKeepSharedBg`. Returns the screen. */

UiScreen *UiCollectionCardCtor(UiCollectionCard *self, s8 category)
{
  bool fromLow;
  void *data;
  GfxFader *fader;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiCollectionCardVtbl;
  self->category = category;
  UiCollectionCardInitState(self);
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(66 * sizeof(GfxSprite *), NULL, 0);
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
  return &self->base;
}
