// bdc 0x08949380 UiBattleRecordOpenPhase
#include "bdc.h"

/* Phase 3 of `UiBattleRecord`, one step per call (`base.phaseStep`):
   step 0 builds the category menu (`UiBattleRecordBuildMenu`) and plays `"main_start.fab"` in
   shared animation slot 1; step 1 waits until that animation reaches its length, then swaps slot 1
   to the looping `"main_light.fab"` (two `GfxFabUpdate` ticks), flips the title plate (sprite 15
   of `base.data`) open (`UiTitlePlateInit`), sets its `layerMask` to 1 and resets `animFrame`;
   step 2 animates the menu in (`UiBattleRecordAnimateMenu``(…, 0)`) and resets `animFrame`
   once done; step 3 enters phase 2 (the menu phase, `UiBattleRecordMenuPhase`) at step 0.
   Other steps do nothing. */

void UiBattleRecordOpenPhase(UiBattleRecord *self)
{
  switch (self->base.phaseStep) {
  case 0:
    UiBattleRecordBuildMenu(self);
    UiSharedAnimStart(20.0f, 0.0f, 0.0f, self, (void *)"main_start.fab", 1, 0);
    self->base.phaseStep = 1;
    break;
  case 1: {
    s32 frame = (s32)UiSharedAnimGetFrame(self, 1);
    if (frame < (s32)UiSharedAnimGetLength(self, 1)) {
      break;
    }
    UiSharedAnimRelease(self, 1);
    UiSharedAnimStart(20.0f, 0.0f, 0.0f, self, (void *)"main_light.fab", 1, 1);
    GfxFabUpdate(g_uiSharedAnims[1]);
    GfxFabUpdate(g_uiSharedAnims[1]);
    UiTitlePlateInit(0, ((GfxSprite **)self->base.data)[15]);
    ((GfxSprite **)self->base.data)[15]->layerMask = 1;
    self->base.phaseStep = 2;
    self->animFrame = 0;
    break;
  }
  case 2:
    if (UiBattleRecordAnimateMenu(self, 0) != 0) {
      self->base.phaseStep = 3;
      self->animFrame = 0;
    }
    break;
  case 3:
    self->base.phase = 2;
    self->base.phaseStep = 0;
    break;
  }
}
