// bdc 0x08934040 UiGauntletSetupCardListDone
#include "bdc.h"

/* Advances the card-list tweens of the gauntlet setup screen (task 373, `maybe_UiScreen373Ctor`,
   class prefix `UiGauntletSetup`; card sprites `cc_card_L_%03d`, `c_set_OK_bo_*`, texts
   `DWCardName`/`DWCardHelp`; main update `UiGauntletSetupMainPhase`; focus area `+0x74`, item `+0x76`):
   sprites/tweens 7..9, 48..49 and 40..41 are run through `UiTweenUpdate` (scale 0->1 when showing,
   1->0 when hiding, 16 frames, flags 3), then the list is re-laid out
   (`UiGauntletSetupLayoutCardList`). Returns true if the byte-wrapped sum of the UiTweenUpdate
   results is non-zero. */

bool UiGauntletSetupCardListDone(UiGauntletSetup *self, u8 hide)
{
  u8 count;
  int i;

  count = 0;
  if (hide == 0) {
    for (i = 7; i < 10; i++) {
      count += UiTweenUpdate(0.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 3);
    }
    for (i = 48; i < 50; i++) {
      count += UiTweenUpdate(0.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 3);
    }
    for (i = 40; i < 42; i++) {
      count += UiTweenUpdate(0.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 3);
    }
  } else {
    for (i = 7; i < 10; i++) {
      count += UiTweenUpdate(1.0f, 0.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 3);
    }
    for (i = 48; i < 50; i++) {
      count += UiTweenUpdate(1.0f, 0.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 3);
    }
    for (i = 40; i < 42; i++) {
      count += UiTweenUpdate(1.0f, 0.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 3);
    }
  }
  /* v0 = (count != 0) is set before this call; the callee leaves v0 untouched. */
  UiGauntletSetupLayoutCardList(self);
  return count != 0;
}
