// bdc 0x0896ac78 UiCardEquipSetRowFocus
#include "bdc.h"

/* Moves the cursor highlight of `UiCardEquip` to row `row` (`focused` = 1) or
   clears it (0): row 0 highlights the selected tab (glow sprite, add colour 0.3, tab texture via
   `UiCardEquipSetTabTexture`) and resets the others; row 1 shows the card cursor (group 18) on
   the current card of the selected Bakugan, the gauge arrows and refreshes every card group; row 2
   focuses the G-power gauge (arrow tint 1.0, else 0.5) and clears the gauge-limit record
   (`gaugeLimit`, 0x14 bytes). Other rows only reset the glow and `tabScale`. */

void UiCardEquipSetRowFocus(UiCardEquip *self, u8 row, u8 focused)
{
    GfxSprite **sprites = (GfxSprite **)self->base.data;
    GfxSprite *spr;
    s32 i;
    s32 k;

    UiCursorGlowReset();
    self->tabScale = 0.0f;

    if (row == 0) {
        for (i = 0; i < self->tabCount; i++) {
            if (i == self->rowCursor[row]) {
                if (focused != 0) {
                    UiPulseReset(&self->pulses[self->groups[3][0] + i]);
                    sprites[self->groups[3][0] + i]->flags |= 1u;
                    GfxSpriteCenterPivot(sprites[self->groups[3][0] + i]);
                    UiSpriteSetScaleRotation(sprites[self->groups[3][0] + i], 1.0f, 1.0f, 0.0f);
                    sprites[self->groups[3][0] + i]->alpha = 1.0f;
                    spr = sprites[self->groups[3][0] + i];
                    for (k = 0; k < 3; k++) {
                        spr->addColor[k] = 0.3f;
                    }
                    spr->addColor[3] = 1.0f;
                    sprites[self->groups[3][0] + i]->posZ = self->spriteDepth[self->groups[3][0] + i];

                    UiCardEquipSetTabTexture(self, sprites[self->groups[4][0] + i], 1, (u8)i);
                    UiSpriteSetScaleRotation(sprites[self->groups[4][0] + i], 1.0f, 1.0f, 0.0f);
                    spr = sprites[self->groups[4][0] + i];
                    for (k = 0; k < 3; k++) {
                        spr->addColor[k] = 0.3f;
                    }
                    spr->addColor[3] = 1.0f;
                    sprites[self->groups[4][0] + i]->posZ = self->spriteDepth[self->groups[4][0] + i];

                    UiSpriteSetScaleRotation(sprites[self->groups[5][0] + i], 1.0f, 1.0f, 0.0f);
                    sprites[self->groups[5][0] + i]->posZ = self->spriteDepth[self->groups[5][0] + i];

                    UiPulseInit(sprites[self->groups[3][0] + i], sprites[self->highlightSprite],
                                &self->pulses[self->highlightSprite]);
                    continue;
                }
                sprites[self->highlightSprite]->flags &= ~1u;
            }
            sprites[self->groups[3][0] + i]->flags &= ~1u;
            UiCardEquipSetTabTexture(self, sprites[self->groups[4][0] + i], 0, (u8)i);
            UiSpriteSetScaleRotation(sprites[self->groups[4][0] + i], 1.0f, 1.0f, 0.0f);
            spr = sprites[self->groups[4][0] + i];
            for (k = 0; k < 3; k++) {
                spr->addColor[k] = 0.0f;
            }
            spr->addColor[3] = 1.0f;
            sprites[self->groups[4][0] + i]->posZ = self->spriteDepth[self->groups[4][0] + i];
            UiSpriteSetScaleRotation(sprites[self->groups[5][0] + i], 1.0f, 1.0f, 0.0f);
            sprites[self->groups[5][0] + i]->posZ = self->spriteDepth[self->groups[5][0] + i];
        }
    } else if (row < 2) {
        sprites[self->groups[11][0] + self->selBakugan]->flags |= 1u;
        sprites[self->groups[12][0] + self->selBakugan]->flags &= ~1u;
        if (focused != 0) {
            UiPulseReset(&self->pulses[self->groups[18][0]]);
            sprites[self->groups[18][0]]->flags |= 1u;
            GfxSpriteCenterPivot(sprites[self->groups[18][0]]);
            UiSpriteSetScaleRotation(sprites[self->groups[18][0]], 1.0f, 1.0f, 0.0f);
            sprites[self->groups[18][0]]->alpha = 1.0f;
            spr = sprites[self->groups[18][0]];
            spr->addColor[3] = 1.0f;
            for (k = 0; k < 3; k++) {
                spr->addColor[k] = 0.3f;
            }
            sprites[self->groups[18][0]]->posX =
                sprites[self->groups[8][0] + self->selBakugan * 4 + self->rowCursor[row]]->posX;
            sprites[self->groups[18][0]]->posY =
                sprites[self->groups[8][0] + self->selBakugan * 4 + self->rowCursor[row]]->posY;
            sprites[self->groups[18][0]]->posZ = self->spriteDepth[self->groups[18][0]];
            UiCardEquipSetGaugeArrowTint(0.5f, &self->base, 1, (u8)self->selBakugan);
            UiCardEquipGreyGaugeLimits(self, (u8)self->selBakugan);
            UiPulseInit(sprites[self->groups[18][0]], sprites[self->highlightSprite],
                        &self->pulses[self->highlightSprite]);
        } else {
            sprites[self->groups[18][0]]->flags &= ~1u;
            UiCardEquipSetGaugeArrowTint(0.5f, &self->base, 0, (u8)self->selBakugan);
            sprites[self->highlightSprite]->flags &= ~1u;
        }
        sprites[self->groups[15][0] + self->selBakugan * 2]->alpha = 1.0f;
        UiCardEquipRefreshCardIcons(self);
        UiCardEquipRefreshActiveMarksB(self);
        UiCardEquipRefreshCardMarkers(self);
        UiCardEquipRefreshActiveMarksA(self);
        UiCardEquipRefreshEmptySlots(self);
    } else if (row < 3) {
        sprites[self->groups[11][0] + self->selBakugan]->flags |= 1u;
        sprites[self->groups[12][0] + self->selBakugan]->flags &= ~1u;
        sprites[self->groups[15][0] + self->selBakugan * 2]->alpha = 1.0f;
        if (focused != 0) {
            UiCardEquipSetGaugeArrowTint(1.0f, &self->base, 1, (u8)self->selBakugan);
        } else {
            UiCardEquipSetGaugeArrowTint(0.5f, &self->base, 1, (u8)self->selBakugan);
        }
        UiCardEquipGreyGaugeLimits(self, (u8)self->selBakugan);
        memset(&self->gaugeLimit, 0, 0x14);
    }
}
