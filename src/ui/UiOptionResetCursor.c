// bdc 0x089720a8 UiOptionResetCursor
#include "bdc.h"

/* Resets the highlight of the battle-options screen (task 304, `maybe_UiScreen304Ctor`;
   `"option_battle_t_%02d"` / `"option_sol00"` sprites; edits the battle rules stored in profile
   words 0x18..0x1b) onto the selected row `cursor`: rows 0..3 are value rows (plates 0x1f..0x22,
   `UiOptionSetRowPlateTexture`, pulse slots 0x1f..), rows 4/5 the OK / Defaults buttons (sprites
   0x30/0x31 with labels 0x32/0x33, `UiOptionSetButtonTexture`; scale and depth restored). With
   the cursor on a value row the cursor ghost sprites 0x34/0x3a are hidden; on a button, sprite
   0x34 is shown centred on it with a 0.3 colour add and a cursor pulse is started (slot 0x3a). */

void UiOptionResetCursor(UiOption *self)
{
    GfxSprite *ghost;
    int i;
    u8 sel;

    UiCursorGlowReset();
    self->zoomFrame = 0.0f;
    for (i = 0; i < 6; i++) {
        sel = (i == (s8)self->cursor) ? 1 : 0;
        if (i == 4 || i == 5) {
            UiOptionSetButtonTexture(self, ((GfxSprite **)self->base.data)[i + 0x2c], sel);
            UiSpriteSetHighlight(((GfxSprite **)self->base.data)[i + 0x2c], sel);
            memset(&self->slots[i + 0x2c], 0, sizeof(UiTweenSlot));
            UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i + 0x2c], 1.0f, 1.0f, 0.0f);
            ((GfxSprite **)self->base.data)[i + 0x2c]->posZ = self->spriteDepth[i + 0x2c];
            UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i + 0x2e], 1.0f, 1.0f, 0.0f);
            ((GfxSprite **)self->base.data)[i + 0x2e]->posZ = self->spriteDepth[i + 0x2e];
        } else {
            UiOptionSetRowPlateTexture(self, ((GfxSprite **)self->base.data)[i + 0x1f], sel);
            UiSpriteSetHighlight(((GfxSprite **)self->base.data)[i + 0x1f], sel);
            memset(&self->slots[i + 0x1f], 0, sizeof(UiTweenSlot));
        }
    }

    if ((s8)self->cursor < 4) {
        ((GfxSprite **)self->base.data)[0x34]->flags &= ~1u;
        ((GfxSprite **)self->base.data)[0x3a]->flags &= ~1u;
        return;
    }

    memset(&self->slots[0x34], 0, sizeof(UiTweenSlot));
    ((GfxSprite **)self->base.data)[0x34]->flags |= 1;
    GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[0x34]);
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[0x34], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[0x34]->alpha = 1.0f;
    ghost = ((GfxSprite **)self->base.data)[0x34];
    ghost->addColor[3] = 1.0f;
    ghost->addColor[0] = 0.3f;
    ghost->addColor[1] = 0.3f;
    ghost->addColor[2] = 0.3f;
    ((GfxSprite **)self->base.data)[0x34]->posX =
        ((GfxSprite **)self->base.data)[(s8)self->cursor + 0x2c]->posX;
    ((GfxSprite **)self->base.data)[0x34]->posY =
        ((GfxSprite **)self->base.data)[(s8)self->cursor + 0x2c]->posY;
    ((GfxSprite **)self->base.data)[0x34]->posZ = self->spriteDepth[0x34];
    UiPulseInit(((GfxSprite **)self->base.data)[0x34], ((GfxSprite **)self->base.data)[0x3a],
                &self->slots[0x3a].pulse);
}
