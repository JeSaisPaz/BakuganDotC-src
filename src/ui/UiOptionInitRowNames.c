// bdc 0x08970b44 UiOptionInitRowNames
#include "bdc.h"

/* Shows the row name sprites 0x28..0x2b of `UiOption`, greyed out (tint 0.5) for
   rows disabled in `enabledRows`. */

void UiOptionInitRowNames(UiOption *self)
{
    int i;

    for (i = 40; i < 44; i++) {
        UiOptionShowSprite(&self->base, ((GfxSprite **)self->base.data)[i]);
        ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
        self->spriteY[i] = ((GfxSprite **)self->base.data)[i]->posY;
        if (!(self->enabledRows & (1 << (i - 40)))) {
            GfxSprite *s = ((GfxSprite **)self->base.data)[i];
            s->tint[0] = 0.5f;
            s->tint[1] = 0.5f;
            s->tint[2] = 0.5f;
            s->alpha = 1.0f;
        }
    }
}
