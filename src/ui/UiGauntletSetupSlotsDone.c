// bdc 0x0893471c UiGauntletSetupSlotsDone
#include "bdc.h"

/* Advances the slot-panel tweens started by `UiGauntletSetupTweenCardSlots` on the gauntlet setup
   screen (task 373, `maybe_UiScreen373Ctor`, class prefix `UiGauntletSetup`; card sprites
   `cc_card_L_%03d`, `c_set_OK_bo_*`, texts `DWCardName`/`DWCardHelp`; main update `UiGauntletSetupMainPhase`;
   focus area `+0x74`, item `+0x76`): sprites/tweens 14..17, 42..45, 34..37, 50..53, 24..27, 10..13
   and 20..23 (in that order) are run through `UiTweenUpdate` (scale 1->1, 16 frames, flags 5,
   `hide` as fade-out). Returns true if the byte-wrapped sum of the UiTweenUpdate results is
   non-zero. */

bool UiGauntletSetupSlotsDone(UiGauntletSetup *self, u8 hide)
{
  u8 count;
  int i;

  count = 0;
  for (i = 14; i < 18; i++) {
    count += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                           &self->tweens[i], 5);
  }
  for (i = 42; i < 46; i++) {
    count += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                           &self->tweens[i], 5);
  }
  for (i = 34; i < 38; i++) {
    count += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                           &self->tweens[i], 5);
  }
  for (i = 50; i < 54; i++) {
    count += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                           &self->tweens[i], 5);
  }
  for (i = 24; i < 28; i++) {
    count += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                           &self->tweens[i], 5);
  }
  for (i = 10; i < 14; i++) {
    count += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                           &self->tweens[i], 5);
  }
  for (i = 20; i < 24; i++) {
    count += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                           &self->tweens[i], 5);
  }
  return count != 0;
}
