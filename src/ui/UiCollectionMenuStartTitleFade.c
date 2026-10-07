// bdc 0x08975db0 UiCollectionMenuStartTitleFade
#include "bdc.h"

/* Starts the fade tween (`UiTweenBegin`) of the title sprites 0x12/0x13 of
   `UiCollectionMenu` (shown first when opening). */

void UiCollectionMenuStartTitleFade(UiCollectionMenu *self, u8 out)
{
    int i;

    if (out == 0) {
        for (i = 0x12; i < 0x14; i++) {
            ((GfxSprite **)self->base.data)[i]->flags |= 1;
            UiTweenBegin(1.0f, 0, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        }
    } else {
        for (i = 0x12; i < 0x14; i++) {
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        }
    }
}
