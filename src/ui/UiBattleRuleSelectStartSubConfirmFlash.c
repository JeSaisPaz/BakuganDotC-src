// bdc 0x0895422c UiBattleRuleSelectStartSubConfirmFlash
#include "bdc.h"

/* Starts the confirm flash (channel 0, 2 frames) on the focused sub-option button of
   `UiBattleRuleSelect` (sprite 0x0b/0x0e + `+0xa44`). */

void UiBattleRuleSelectStartSubConfirmFlash(UiBattleRuleSelect *self)
{
    int idx;

    if (SaveGetProfileFlag0()) {
        idx = self->subCursor + 0xe;
    } else {
        idx = self->subCursor + 0xb;
    }
    UiFlashStart(2.0f, ((GfxSprite **)self->base.data)[idx], 0, 0);
}
