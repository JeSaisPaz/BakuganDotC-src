// bdc 0x0896d7bc UiCardEquipRefreshGaugeDigits
#include "bdc.h"

/* Writes the G-power value of every Bakugan of `UiCardEquip` into its three digit
   sprites (group 13, cells via `UiNumberToDigits`, leading digit hidden below 100) and shows the
   unit sprite. Group 13 holds 4 sprites per Bakugan: hundreds, tens, ones, unit; only the first
   bakuganCount*4 are touched. */

void UiCardEquipRefreshGaugeDigits(UiCardEquip *self)
{
    u8 digits[4][4];
    u8 len[4];
    int i;
    int s;

    memset(digits, 0, sizeof(digits));
    for (i = 0; i < 4; i++) {
        u8 n;

        UiNumberToDigits(digits[i], self->gauge[i], 3, 0xff);
        len[i] = 0;
        n = len[i];
        while (digits[i][n] != 0xff) {
            n++;
        }
        len[i] = n;
    }

    for (s = self->groups[13][0]; s < self->groups[13][0] + (s8)self->groups[13][1]; s++) {
        GfxSprite **sprites = (GfxSprite **)self->base.data;
        int rel = s - self->groups[13][0];
        int slot;
        int k;
        u8 digit;

        if (rel >= self->bakuganCount * 4)
            continue;
        slot = rel % 4;
        if (slot == 3) {
            /* unit sprite */
            UiCardEquipShowSprite(self, sprites[s]);
            ((GfxSprite **)self->base.data)[s]->alpha = 1.0f;
            continue;
        }
        if (slot < 0)
            continue;
        if (slot == 0) {
            /* hundreds: hidden unless the value has 3 digits */
            GfxSprite *sprite = sprites[s];

            if (len[rel / 4] < 3) {
                sprite->flags &= ~1u;
                continue;
            }
            UiCardEquipShowSprite(self, sprite);
            ((GfxSprite **)self->base.data)[s]->alpha = 1.0f;
            k = (s - self->groups[13][0]) / 4;
            digit = digits[k][0];
        } else if (slot == 1) {
            /* tens: hidden unless the value has at least 2 digits */
            GfxSprite *sprite = sprites[s];

            if (len[rel / 4] < 2) {
                sprite->flags &= ~1u;
                continue;
            }
            UiCardEquipShowSprite(self, sprite);
            ((GfxSprite **)self->base.data)[s]->alpha = 1.0f;
            k = (s - self->groups[13][0]) / 4;
            digit = digits[k][len[k] - 2];
        } else {
            /* ones: always shown */
            UiCardEquipShowSprite(self, sprites[s]);
            ((GfxSprite **)self->base.data)[s]->alpha = 1.0f;
            k = (s - self->groups[13][0]) / 4;
            digit = digits[k][len[k] - 1];
        }
        GfxSpriteSetCell(((GfxSprite **)self->base.data)[s], (float)(digit / 5), (float)(digit % 5));
    }
}
