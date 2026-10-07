// bdc 0x089afbd8 UiBattleModeSelectCtor
#include "bdc.h"

/* Constructor of UiBattleModeSelect, the battle-mode select of the main menu (task 350, object
   size 0x9bc). Runs `UiScreenCtor`, installs `g_uiBattleModeSelectVtbl`, allocates the
   0x30-byte sprite table (`data`, +0x1c) from the low heap, sets frame mode 1
   (`UiScreenSetFrameMode`), creates the fader system if needed (sort key 20000), clears the
   clear colour to opaque black, makes the stick emulate the d-pad, asks a running NetPlay session
   to abort, clears profile flags 0x7eff, refreshes the profile flag 0, zeroes the switch record
   and sets the shared-background hand-over flag `g_uiKeepSharedBg`. Returns `self`. */

UiBattleModeSelect *UiBattleModeSelectCtor(UiBattleModeSelect *self)
{
  bool fromLow;
  void *data;
  GfxFader *fader;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiBattleModeSelectVtbl;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(12 * sizeof(GfxSprite *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->base.data = data;
  UiScreenSetFrameMode((CoreTask *)self, 1);
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
  if (NetPlayHasManager()) {
    NetPlayRequestAbort(NetPlayGetManager());
  }
  if (SaveHasProfile()) {
    SaveProfileClearFlags(SaveGetProfile(), 0x7eff);
  }
  SaveRefreshProfileFlag0();
  memset(&self->switchDir, 0, 0xc);
  g_uiKeepSharedBg = 1;
  return self;
}
