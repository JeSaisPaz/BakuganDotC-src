// bdc 0x089485b0 UiBattleRecordCtor
#include "bdc.h"

/* Constructor of the task class created for task id 3005 (0xbbd) by `CoreTaskNewById` (object
   size 0x174, base constructor `UiScreenCtor`, vtable `g_uiBattleRecordVtbl`). The class is the
   battle-record screen (per-Bakugan win/loss statistics per battle mode and in total, layout
   package `"data/2d/<lang>/record.lzs"`), whose methods are named `UiBattleRecord*`: phase table
   `0x08a9d19c` = `UiBattleRecordLoadPhase`, `UiBattleRecordFinishPhase`,
   `UiBattleRecordMenuPhase`, `UiBattleRecordOpenPhase`, `UiBattleRecordModeListPhase`,
   `UiBattleRecordTotalListPhase`; `UiBattleRecordDtor`, `UiBattleRecordDraw`.
   Clears the state words and the four statistics arrays, allocates the zeroed 8-byte shared
   animation table `g_uiSharedAnims` from the low heap, clears `bgAnimList`/`unk58`/`unk5c`,
   fills `owned[i]` with profile bit i+1 of `bakuganBitsA` (non-zero = owned), makes the stick
   emulate the d-pad and hands the shared background over (`UiScreenKeepSharedBg`).
   Returns `self`. */

UiBattleRecord *UiBattleRecordCtor(UiBattleRecord *self)
{
  bool fromLow;
  void *anims;
  SaveProfile *profile;
  int i;
  int bit;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiBattleRecordVtbl;
  self->package = NULL;
  self->animFrame = 0;
  self->mode = 0;
  self->scrollTop = 0;
  self->scrollDir = 0;
  self->scrollFrame = 0;
  memset(self->wins, 0, sizeof(self->wins));
  memset(self->losses, 0, sizeof(self->losses));
  memset(self->draws, 0, sizeof(self->draws));
  memset(self->battles, 0, sizeof(self->battles));
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  anims = MemAlloc(2 * sizeof(GfxFab *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_uiSharedAnims = (GfxFab **)anims;
  memset(anims, 0, 2 * sizeof(GfxFab *));
  self->base.unk58 = 0;
  self->base.bgAnimList = NULL;
  self->base.unk5c = 0;
  for (i = 0; i < 20; i++) {
    profile = SaveGetProfile();
    bit = i + 1;
    self->owned[i] = (u8)(profile->data->bakuganBitsA[bit / 8] & (1 << (bit % 8)));
  }
  self->base.pad->stickEmulatesDpad = 1;
  self->mode = 0;
  UiScreenKeepSharedBg(&self->base);
  return self;
}
