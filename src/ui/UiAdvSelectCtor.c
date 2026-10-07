// bdc 0x089176f0 UiAdvSelectCtor
#include "bdc.h"

/* Constructor of the adventure character/Bakugan select screen, task id 376 (0x178) (base
   `UiScreenCtor`, vtable `0x08af49d4`). Object size 0x984. Marks the six candidate
   Bakugan (`UiAdvSelectGetCandidateId`) as owned in the save profile (`ownedBakugan` bit set),
   initialises the selection state
   (`UiAdvSelectInitState`), allocates the 40-sprite table (`base.data`, 0xa0 bytes) and a one-slot
   `g_uiSharedAnims` table from the low heap, primes the fader, sets a black clear colour, saves
   and forces the pad's `stickEmulatesDpad`, and keeps the shared background. Returns `self`. Content (from
   its strings): `"adv_chara_%02d"` character portraits, `"adv_baku_%02d"`/`"adv_baku_shadow%02d"`
   Bakugan images, `"adv_yaji_01/02"` arrows, `"f_cha_name_baku_%02d"` names, `"menu_daiza.gmo"`
   pedestal, `"main_bg.fab"` background and help text `"DWMesHelp"`. The exact role (adventure
   partner selection) is inferred from the `adv_` asset names. */

UiAdvSelect *UiAdvSelectCtor(UiAdvSelect *self)
{
  SaveProfile *profile;
  bool fromLow;
  void *mem;
  PadState *pad;
  int id;
  int i;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiAdvSelectVtbl;
  for (i = 0; i < 6; i++) {
    profile = SaveGetProfile();
    id = UiAdvSelectGetCandidateId(self, true, (u8)i);
    profile->data->ownedBakugan[id / 8] |= 1 << (id % 8);
  }
  UiAdvSelectInitState(self);
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(40 * sizeof(GfxSprite *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->base.data = mem;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(GfxFab *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_uiSharedAnims = (GfxFab **)mem;
  memset(mem, 0, sizeof(GfxFab *));
  UiScreenSetFrameMode((CoreTask *)self, 0);
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
  pad = self->base.pad;
  self->savedStickEmulatesDpad = pad->stickEmulatesDpad;
  pad->stickEmulatesDpad = 1;
  UiScreenKeepSharedBg(&self->base);
  return self;
}
