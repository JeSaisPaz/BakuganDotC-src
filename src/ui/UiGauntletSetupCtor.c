// bdc 0x08931a5c UiGauntletSetupCtor
#include "bdc.h"

/* Constructor of the gauntlet card setup screen, task id 373 (0x175) (base `UiScreenCtor`, vtable
   `g_uiGauntletSetupVtbl`). Object size 0x1b60. Clears its state (`UiGauntletSetupResetState`),
   allocates the 0xf0-byte (60-entry) sprite table `data` from the low heap, sets frame mode 0, zeroes
   `word6c`/`word70`, creates the fader system if it is not ready yet (default fader sort key 20000),
   sets the clear colour to opaque black, saves the pad's `stickEmulatesDpad` and forces it on, then
   creates the card-name and card-help text boxes (`UiGauntletSetupCreateNameBox`,
   `UiGauntletSetupCreateHelpBox`). Returns `self`. Content (strings): card list sprites
   `"cc_card_L_%03d"`/`"cc_cus_non_card"`, `"c_set_OK_bo_1/2"` buttons,
   `"DWCardName"`/`"DWCardHelp"`/`"DWHologramHelp"` texts, the avatar models `"12_Edit_man.gmo"`,
   `"12_editm_see_gauntlet(_push).gmo"` (player looking at the gauntlet) and `"menu_daiza.gmo"`. */

UiGauntletSetup *UiGauntletSetupCtor(UiGauntletSetup *self)
{
  bool fromLow;
  void *sprites;
  PadState *pad;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiGauntletSetupVtbl;
  UiGauntletSetupResetState(self);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprites = MemAlloc(60 * sizeof(GfxSprite *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->base.data = sprites;

  UiScreenSetFrameMode((CoreTask *)self, 0);
  self->word6c = 0;
  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit(NULL);
    GfxGetActiveFader()->sortKey = 20000.0f;
  }
  self->word70 = 0;

  g_gfxDisplay->clearColor[0] = 0.0f;
  g_gfxDisplay->clearColor[1] = 0.0f;
  g_gfxDisplay->clearColor[2] = 0.0f;
  g_gfxDisplay->clearColor[3] = 1.0f;

  pad = self->base.pad;
  self->savedStickEmulatesDpad = pad->stickEmulatesDpad;
  pad->stickEmulatesDpad = 1;

  UiGauntletSetupCreateNameBox(self);
  UiGauntletSetupCreateHelpBox(self);
  return self;
}
