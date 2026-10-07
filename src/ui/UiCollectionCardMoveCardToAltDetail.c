// bdc 0x08986a14 UiCollectionCardMoveCardToAltDetail
#include "bdc.h"

/* Same card movement as `UiCollectionCardMoveCardToDetail` for the alternative detail view of the
   card collection screen (task 313, `maybe_UiScreen313Ctor`; pages of ability cards
   `"collection_ability_%02d"` in `"waku_4_a"`/`"waku_4_b"` frames, large card art `"card_L_%03d"`,
   help text `"DWCardHelp"`). Advances the selected card's tween (tweens[13 + cursor]) by 1/16 and
   eases its sprite (sprite 13 + cursor) from `slideStart/slideEnd` by the detail offsets and from
   `detailScale` towards `detailScaleEnd`; `back` = 0 eases out (towards), nonzero eases in (t*t).
   Once t >= 1 the sprite snaps to `slideDelta/moveToY` and `detailScaleEnd` and it returns true;
   otherwise false. Either way scaleY = scaleX, angle = 0 and the matrix is rebuilt. */

#define CARD_SPRITE(self) (((GfxSprite **)(self)->base.data)[13 + (self)->cursor])
#define CARD_TWEEN(self) ((self)->tweens[13 + (self)->cursor])

bool UiCollectionCardMoveCardToAltDetail(UiCollectionCard *self, bool back)
{
    bool done = false;
    UiTween *tween = &CARD_TWEEN(self);
    float t = tween->t + 0.0625f;
    float startX = (float)tween->slideStart;
    float offsetX = self->detailOffsetX;
    float u;

    if (!back) {
        tween->t = t;
        CARD_SPRITE(self)->posX = startX + (1.0f - (t - 1.0f) * (t - 1.0f)) * offsetX;
        u = CARD_TWEEN(self).t - 1.0f;
        CARD_SPRITE(self)->posY = (float)CARD_TWEEN(self).slideEnd + (1.0f - u * u) * self->detailOffsetY;
        u = CARD_TWEEN(self).t - 1.0f;
        CARD_SPRITE(self)->scaleX =
            self->detailScale + (1.0f - u * u) * (self->detailScaleEnd - self->detailScale);
    } else {
        tween->t = t;
        CARD_SPRITE(self)->posX = startX + t * t * offsetX;
        u = CARD_TWEEN(self).t;
        CARD_SPRITE(self)->posY = (float)CARD_TWEEN(self).slideEnd + u * u * self->detailOffsetY;
        u = CARD_TWEEN(self).t;
        CARD_SPRITE(self)->scaleX = self->detailScale - u * u * (self->detailScale - self->detailScaleEnd);
    }

    if (!(CARD_TWEEN(self).t < 1.0f)) {
        CARD_SPRITE(self)->posX = (float)CARD_TWEEN(self).slideDelta;
        done = true;
        CARD_SPRITE(self)->posY = (float)CARD_TWEEN(self).moveToY;
        CARD_SPRITE(self)->scaleX = self->detailScaleEnd;
    }
    CARD_SPRITE(self)->scaleY = CARD_SPRITE(self)->scaleX;
    CARD_SPRITE(self)->angle = 0.0f;
    GfxSpriteSetScaleRotation(CARD_SPRITE(self), CARD_SPRITE(self)->scaleX, CARD_SPRITE(self)->scaleY,
                              CARD_SPRITE(self)->angle, false);
    return done;
}
