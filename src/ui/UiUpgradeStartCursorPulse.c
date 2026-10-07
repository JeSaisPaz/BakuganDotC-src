// bdc 0x0891446c UiUpgradeStartCursorPulse
#include "bdc.h"

/* Starts the highlight pulse (`UiPulseStepTint`, period 40) on the cursor sprite of the Bakugan
   upgrade screen (`UiUpgradeCtor`, task 490; selected Bakugan `+0x16a8`, selected slot
   `+0x1698`): sprite `+0xd4` (record `+0x8b4`) when slot 6 (OK button) is selected, else sprite
   `+0xc8` (record `+0x83c`). */

void UiUpgradeStartCursorPulse(UiUpgrade *self)
{
    GfxSprite **sprites = (GfxSprite **)self->base.data;

    if (self->focus == 6) {
        UiPulseStepTint(40.0f, sprites[53], (UiPulse *)&self->tweens[0x35]);
        return;
    }
    UiPulseStepTint(40.0f, sprites[50], (UiPulse *)&self->tweens[0x32]);
}
