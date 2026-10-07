// bdc 0x0896cda4 UiCardEquipBlinkGaugeLimit
#include "bdc.h"

/* Blinks the gauge limit mark (group 12 sprite of the selected Bakugan) in
   `UiCardEquip`: advances `limitBlinkT` by 1/8 per frame and sets the mark's alpha
   to `limitBlinkBase` minus (`limitBlinkUp` = 0) or plus the progress; once the progress reaches 1
   the alpha is set to 1.0, becomes the new base, the progress restarts at 0 and the direction
   flips. The mark's alpha is then copied to the gauge bar (group 15 sprite of the Bakugan). */

void UiCardEquipBlinkGaugeLimit(UiCardEquip *self)
{
    GfxSprite **sprites;
    GfxSprite *mark;
    float t;
    u8 up;

    t = self->limitBlinkT + 0.125f;
    up = self->limitBlinkUp;
    self->limitBlinkT = t;
    mark = ((GfxSprite **)self->base.data)[self->groups[12][0] + self->selBakugan];
    if (up == 0) {
        mark->alpha = self->limitBlinkBase - t;
    } else {
        mark->alpha = self->limitBlinkBase + t;
    }

    sprites = (GfxSprite **)self->base.data;
    mark = sprites[self->groups[12][0] + self->selBakugan];
    if (!(self->limitBlinkT < 1.0f)) {
        mark->alpha = 1.0f;
        sprites = (GfxSprite **)self->base.data;
        self->limitBlinkT = 0.0f;
        self->limitBlinkBase = sprites[self->groups[12][0] + self->selBakugan]->alpha;
        self->limitBlinkUp = (up == 0) ? 1 : 0;
        mark = sprites[self->groups[12][0] + self->selBakugan];
    }
    sprites[self->groups[15][0] + self->selBakugan * 2]->alpha = mark->alpha;
}
