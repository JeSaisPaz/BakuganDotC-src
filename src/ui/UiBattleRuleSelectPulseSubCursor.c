// bdc 0x08953fec UiBattleRuleSelectPulseSubCursor
#include "bdc.h"

/* Per-frame pulse of the sub-option cursor sprite 0x15 of
   `UiBattleRuleSelect` (`UiPulseStepTint`, 20-frame period, state `+0x3c0`).
    */

void UiBattleRuleSelectPulseSubCursor(UiBattleRuleSelect *self)
{
  UiPulseStepTint(20.0f, ((GfxSprite **)self->base.data)[0x15], (UiPulse *)(self->tweens + 0x15));
}
