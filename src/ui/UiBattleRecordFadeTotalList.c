// bdc 0x0894b134 UiBattleRecordFadeTotalList
#include "bdc.h"

/* Fades the totals list of `UiBattleRecord` in (`dir` 0: alpha of sprites
   6–0x50 += 1/6) or out (`dir` 1: sprites 6–0x65 −= 1/6) one step; returns 1 after 7 steps
   (counter `+0x70`), else 0. */

s32 UiBattleRecordFadeTotalList(UiBattleRecord *self, s32 dir)
{
    GfxSprite **sprites = (GfxSprite **)self->base.data;
    int i;

    if (dir == 0) {
        for (i = 6; i < 0x51; i++) {
            sprites[i]->alpha += 0.16666667f;
        }
    } else if (dir == 1) {
        for (i = 6; i < 0x66; i++) {
            sprites[i]->alpha -= 0.16666667f;
        }
    }
    if (self->animFrame > 6) {
        return 1;
    }
    self->animFrame++;
    return 0;
}
