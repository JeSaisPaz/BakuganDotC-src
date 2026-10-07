// bdc 0x089ad944 UiPauseSettingsStartGuideTweens
#include "bdc.h"

/* Starts the tweens (`helpIconTweens`) of the button-guide sprites, layout entries 0x39..0x3c.
   Opening (`closing` == 0): first sets the icons of 0x39 and 0x3a (`UiSetButtonIcon` 2 and 1)
   and shows all four (flags bit 0); closing: only starts the fade-out tweens. Stepped by
   `UiPauseSettingsStepHelpIcons`. */

void UiPauseSettingsStartGuideTweens(UiPauseSettings *self, u8 closing)
{
    int i;

    if (closing == 0) {
        for (i = 0x39; i < 0x3d; i++) {
            if (i < 0x3a) {
                if (i >= 0x39) {
                    UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
                }
            } else if (i < 0x3b) {
                UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 1);
            }
            ((GfxSprite **)self->base.data)[i]->flags |= 1;
            UiTweenBegin(1.0f, closing, ((GfxSprite **)self->base.data)[i],
                         &self->helpIconTweens[i - 0x39], 3);
        }
    } else {
        for (i = 0x39; i < 0x3d; i++) {
            UiTweenBegin(1.0f, closing, ((GfxSprite **)self->base.data)[i],
                         &self->helpIconTweens[i - 0x39], 3);
        }
    }
}
