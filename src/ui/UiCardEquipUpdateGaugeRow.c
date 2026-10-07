// bdc 0x0896e02c UiCardEquipUpdateGaugeRow
#include "bdc.h"

/* Updates the gauge limit mark of the selected Bakugan in `UiCardEquip`.
   `gaugeLimit` 0 (no mark): shows the arrows (group 11) and hides the mark (group 12); if the
   gauge is at 50 or 150 it clears the 0x14-byte limit record, hides the arrows, shows the mark at
   alpha 1.0 with cell (0,0) for 50 or (0,1) for 150, sets `gaugeLimit` to 1/2, takes the mark's
   alpha as `limitBlinkBase` and sets the gauge bar (group 15) alpha to 1.0.
   `gaugeLimit` 1/2: while the gauge stays at 50/150 the mark blinks
   (`UiCardEquipBlinkGaugeLimit`); otherwise the arrows come back, the mark is hidden, the bar
   alpha goes back to 1.0 and `gaugeLimit` to 0. Other values do nothing. */

void UiCardEquipUpdateGaugeRow(UiCardEquip *self)
{
    GfxSprite **sprites;
    GfxSprite *mark;
    u8 limit;
    u8 gauge;

    limit = self->gaugeLimit;
    if (limit == 0) {
        sprites = (GfxSprite **)self->base.data;
        sprites[self->groups[11][0] + self->selBakugan]->flags |= 1;
        sprites = (GfxSprite **)self->base.data;
        sprites[self->groups[12][0] + self->selBakugan]->flags &= ~1u;
        gauge = self->gauge[self->selBakugan];
        if (gauge == 50 || gauge == 150) {
            memset(&self->gaugeLimit, 0, 0x14);
            sprites = (GfxSprite **)self->base.data;
            sprites[self->groups[11][0] + self->selBakugan]->flags &= ~1u;
            sprites = (GfxSprite **)self->base.data;
            sprites[self->groups[12][0] + self->selBakugan]->flags |= 1;
            sprites = (GfxSprite **)self->base.data;
            sprites[self->groups[12][0] + self->selBakugan]->alpha = 1.0f;
            sprites = (GfxSprite **)self->base.data;
            GfxSpriteSetCell(sprites[self->groups[12][0] + self->selBakugan], 0.0f,
                             (gauge == 50) ? 0.0f : 1.0f);
            sprites = (GfxSprite **)self->base.data;
            mark = sprites[self->groups[12][0] + self->selBakugan];
            self->gaugeLimit = (gauge == 50) ? 1 : 2;
            self->limitBlinkBase = mark->alpha;
        }
        if (self->gaugeLimit != 0) {
            sprites = (GfxSprite **)self->base.data;
            sprites[self->groups[15][0] + self->selBakugan * 2]->alpha = 1.0f;
        }
    } else if (limit < 3) {
        if (self->gauge[self->selBakugan] == ((limit == 1) ? 50 : 150)) {
            UiCardEquipBlinkGaugeLimit(self);
        } else {
            sprites = (GfxSprite **)self->base.data;
            sprites[self->groups[11][0] + self->selBakugan]->flags |= 1;
            sprites = (GfxSprite **)self->base.data;
            sprites[self->groups[12][0] + self->selBakugan]->flags &= ~1u;
            sprites = (GfxSprite **)self->base.data;
            sprites[self->groups[15][0] + self->selBakugan * 2]->alpha = 1.0f;
            self->gaugeLimit = 0;
        }
    }
}
