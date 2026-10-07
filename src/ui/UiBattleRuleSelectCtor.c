// bdc 0x089522e0 UiBattleRuleSelectCtor
#include "bdc.h"

/* Constructor of UiBattleRuleSelect, the battle-rule select opened from the main menu's battle-mode
   select (four battle types; commits profile word 7 and words 0x15/0x16, menu result 1..4;
   `menu_daiza.gmo`); task id 340 (0x154), object size 0xa54. Runs `UiScreenCtor`, installs
   `g_uiBattleRuleSelectVtbl`, allocates the 0x74-byte per-screen data block (`data`, +0x1c) from
   the low heap, sets frame mode 1 (`UiScreenSetFrameMode`), creates the fader system if needed
   (sort key 20000), clears the clear colour to opaque black and makes the stick emulate the
   d-pad. Refreshes the profile flag (`SaveRefreshProfileFlag0`); when it is set (network mode)
   and a NetPlay manager exists, makes the remote pad emulate the d-pad too, clears flag 0x1000000,
   sets flags 0x2000000 and stores the local slot in `netPlayer`, then resets the net character
   sync (`NetCharaResetAllSync`). Clears the switch-animation state (+0xa34..+0xa3f), sets the
   shared-background hand-over flag `g_uiKeepSharedBg` and returns `self`. */

UiBattleRuleSelect *UiBattleRuleSelectCtor(UiBattleRuleSelect *self)
{
  bool fromLow;
  void *data;
  GfxFader *fader;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiBattleRuleSelectVtbl;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(0x74, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->base.data = data;
  UiScreenSetFrameMode((CoreTask *)self, 1);
  self->unk06c = 0;
  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit(NULL);
    fader = GfxGetActiveFader();
    fader->sortKey = 20000.0f;
  }
  self->unk070 = 0;
  g_gfxDisplay->clearColor[0] = 0.0f;
  g_gfxDisplay->clearColor[1] = 0.0f;
  g_gfxDisplay->clearColor[2] = 0.0f;
  g_gfxDisplay->clearColor[3] = 1.0f;
  self->base.unk30 = 0;
  self->base.pad->stickEmulatesDpad = 1;
  SaveRefreshProfileFlag0();
  self->base.unk24 = 0;
  self->netPlayer = 0;
  if (SaveGetProfileFlag0() != 0) {
    if (NetPlayHasManager()) {
      ((NetPlay *)NetPlayGetManager())->remotePad->stickEmulatesDpad = 1;
      NetPlayClearFlags((NetPlay *)NetPlayGetManager(), 0x1000000);
      NetPlaySetFlags((NetPlay *)NetPlayGetManager(), 0x2000000);
      self->netPlayer = NetPlayGetLocalSlot((NetPlay *)NetPlayGetManager());
    }
    NetCharaResetAllSync();
  }
  memset(&self->switchDir, 0, 0xc);
  g_uiKeepSharedBg = 1;
  return self;
}
