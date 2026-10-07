// bdc 0x0892b418 UiBakuganSelectCtor
#include "bdc.h"

/* Constructor of UiBakuganSelect, the Bakugan select screen (`*_stay_sel` models on
   `menu_daiza.gmo`, `cha_name_baku_%02d`, `card_SS_%03d`, `f_cus_chara_%02d`); task id 371 (0x173),
   object size 0x1d68. Runs `UiScreenCtor`, installs `g_uiBakuganSelectVtbl`, runs the class
   init `UiBakuganSelectInitState`, allocates the 0x214-byte per-screen sprite table (`data`,
   +0x1c, 133 sprite pointers) from the low heap, sets frame mode 0 (`UiScreenSetFrameMode`),
   creates the fader system if needed (sort key 20000), clears the clear colour to opaque black,
   saves the pad's `stickEmulatesDpad` in `savedStickEmulatesDpad` and forces it on. Returns the
   screen. */

UiBakuganSelect *UiBakuganSelectCtor(UiBakuganSelect *self)
{
  bool fromLow;
  void *data;
  GfxFader *fader;
  PadState *pad;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiBakuganSelectVtbl;
  UiBakuganSelectInitState(self);
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(0x214, NULL, 0);
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
  pad = self->base.pad;
  self->savedStickEmulatesDpad = pad->stickEmulatesDpad;
  pad->stickEmulatesDpad = 1;
  return self;
}
