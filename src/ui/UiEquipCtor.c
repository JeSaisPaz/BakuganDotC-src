// bdc 0x08957374 UiEquipCtor
#include "bdc.h"

/* Constructor of UiEquip, the Bakugan/gear loadout screen before a battle (task 302, 0x52a4
   bytes). Runs `UiScreenCtor`, installs `g_uiEquipVtbl`, initialises the state
   (`UiEquipInitState`), allocates the sprite table `base.data` from the low heap (0x3b4 bytes for
   up to two players, 0x5e0 for four) and the zeroed 8-byte `g_uiSharedAnims` slot table, sets
   frame mode 1 (`UiScreenSetFrameMode`), creates the fader system if needed (active fader sort
   key 20000), sets the clear colour to opaque black, makes the stick emulate the D-pad, refreshes
   the profile flag (`SaveRefreshProfileFlag0`), preloads motions (`UiEquipPreloadMotions`),
   clears both text printers, keeps the shared background (`UiScreenKeepSharedBg`) and clears
   `localPlayer`. When profile flag 0 is set it resets the player records keeping the models
   (`UiEquipResetPlayerRecords`), takes `localPlayer` from the NetPlay session when one exists
   (`NetPlayGetLocalSlot`), puts the grid cursor on that player's pick (`UiEquipMapBakuganIndex`
   table 1) and makes that player the edited one. Clears `gearEditing` and returns the screen. */

UiScreen *UiEquipCtor(UiEquip *self)
{
  bool fromLow;
  void *block;
  GfxFader *fader;
  s32 player;
  int i;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiEquipVtbl;
  UiEquipInitState(self);
  if (self->playerCount < 3) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(0x3b4, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->base.data = block;
  }
  else {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(0x5e0, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->base.data = block;
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(8, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_uiSharedAnims = (GfxFab **)block;
  memset(block, 0, 8);
  UiScreenSetFrameMode((CoreTask *)self, 1);
  self->word6c = 0;
  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit(NULL);
    fader = GfxGetActiveFader();
    fader->sortKey = 20000.0f;
  }
  self->word70 = 0;
  g_gfxDisplay->clearColor[0] = 0.0f;
  g_gfxDisplay->clearColor[1] = 0.0f;
  g_gfxDisplay->clearColor[2] = 0.0f;
  g_gfxDisplay->clearColor[3] = 1.0f;
  self->base.pad->stickEmulatesDpad = 1;
  SaveRefreshProfileFlag0();
  self->base.unk24 = 0;
  UiEquipPreloadMotions(self);
  for (i = 0; i < 2; i++) {
    (&self->namePrinter)[i] = NULL; /* namePrinter, helpPrinter */
  }
  UiScreenKeepSharedBg(&self->base);
  *(s32 *)self->localPlayer = 0;
  if (SaveGetProfileFlag0() != 0) {
    UiEquipResetPlayerRecords(self, 1);
    if (NetPlayHasManager()) {
      *(s32 *)self->localPlayer = NetPlayGetLocalSlot(NetPlayGetManager());
    }
    player = *(s32 *)self->localPlayer;
    self->gridCursor = UiEquipMapBakuganIndex(self, 1, self->bakuganPick[player]);
    self->editPlayer = (s8)*(s32 *)self->localPlayer;
  }
  self->gearEditing = 0;
  return &self->base;
}
