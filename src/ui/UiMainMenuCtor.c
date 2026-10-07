// bdc 0x089a39fc UiMainMenuCtor
#include "bdc.h"

/* Constructor of UiMainMenu, the main menu (task 300, object size 0xe50; 3D menu objects
   `menu_worldmap.gmo`, `menu_itembox.gmo`, `menu_gauntlet.gmo`, `menu_credit.gmo` on
   `menu_daiza.gmo`). Runs `UiScreenCtor`, installs `g_uiMainMenuVtbl`, constructs the camera
   (`GfxCameraCtor`), aborts a running NetPlay session (`NetPlayRequestAbort`), clears profile
   flags 0x7eff and refreshes flag 0, resets the local net player index to -1, allocates the
   0x58-byte screen data block (`data`) and the 8-byte zeroed `g_uiSharedAnims` (both low heap),
   sets frame mode 1, creates the fader (sort key 20000) when none is ready, sets the clear colour
   to opaque black, lets the stick act as d-pad, keeps the shared background
   (`UiScreenKeepSharedBg`), zeroes the model/move/light state, restores the save snapshot,
   clears profile word 0x13 and words 0x1f..0x2a, resets the menu result to 0 and creates the
   help line (`UiHelpLineCreate`). Returns `self`. */

UiMainMenu *UiMainMenuCtor(UiMainMenu *self)
{
  bool fromLow;
  void *block;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiMainMenuVtbl;
  GfxCameraCtor((CoreNode *)self->camera);
  if (NetPlayHasManager()) {
    NetPlayRequestAbort(NetPlayGetManager());
  }
  if (SaveHasProfile()) {
    SaveProfileClearFlags(SaveGetProfile(), 0x7eff);
  }
  SaveRefreshProfileFlag0();
  NetSetLocalPlayerIndex(-1);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(22 * sizeof(GfxSprite *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->base.data = block;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(2 * sizeof(GfxFab *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_uiSharedAnims = block;
  memset(block, 0, 2 * sizeof(GfxFab *));

  UiScreenSetFrameMode((CoreTask *)self, 1);
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
  self->base.unk30 = 0;
  self->base.pad->stickEmulatesDpad = 1;
  UiScreenKeepSharedBg(&self->base);
  memset(self->models, 0, sizeof(self->models));
  memset(&self->baseModel, 0, 4);
  memset(&self->moveDir, 0, 0xc);   /* moveDir .. moveT */
  memset(&self->lightMode, 0, 0xc); /* lightMode .. lightPulse */
  self->leave = 0;
  SaveSnapshotRestore();
  if (SaveHasProfile()) {
    SaveProfileSetWord(SaveGetProfile(), 0x13, 0);
    SaveProfileClearWords1fTo2a(SaveGetProfile());
  }
  UiSetMenuResult(&self->base, 0);
  self->bobbing = 0;
  self->leaveAlt = 0;
  UiHelpLineCreate();
  return self;
}
