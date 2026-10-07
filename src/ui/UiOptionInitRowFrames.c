// bdc 0x08970c30 UiOptionInitRowFrames
#include "bdc.h"

/* Shows the row frame sprites 0x2c..0x2f of `UiOption`, greyed out for rows disabled
   in `enabledRows`. */

void UiOptionInitRowFrames(UiOption *self)
{
    int i;

    for (i = 44; i < 48; i++) {
        UiOptionShowSprite(&self->base, ((GfxSprite **)self->base.data)[i]);
        ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
        self->spriteY[i] = ((GfxSprite **)self->base.data)[i]->posY;
        if (!(self->enabledRows & (1 << (i - 44)))) {
            GfxSprite *s = ((GfxSprite **)self->base.data)[i];
            s->tint[0] = 0.5f;
            s->tint[1] = 0.5f;
            s->tint[2] = 0.5f;
            s->alpha = 1.0f;
        }
    }
}
