// bdc 0x0895344c UiBattleRuleSelectStartCursorMove
#include "bdc.h"

/* Starts the button swap of `UiBattleRuleSelect` after a cursor move:
   flashes the arrow sprite 9+`switchDir` (`UiFlashStart`, 2 frames), clears tween 20's toggle,
   hides the old button's glow, and sets up the slides of the outgoing button (sprite/tween
   1+prevCursor, off to X 704 when `switchDir` is 0, else −224) and the incoming one (sprite/tween
   1+cursor, placed at X −224 / 704 and sliding to `panelHomeX`) with their distances. */

void UiBattleRuleSelectStartCursorMove(UiBattleRuleSelect *self)
{
    GfxSprite **sprites;
    UiTween *tw;
    s16 end;

    UiFlashStart(2.0f, ((GfxSprite **)self->base.data)[9 + self->switchDir], 0, 0);
    self->tweens[20].toggle07 = 0;
    UiBattleRuleSelectResetButtonGlow(self, 0, (u8)self->prevCursor);

    /* outgoing button */
    sprites = (GfxSprite **)self->base.data;
    tw = &self->tweens[self->prevCursor + 1];
    tw->slideStart = (s16)sprites[self->prevCursor + 1]->posX;
    end = -224;
    if (self->switchDir == 0) {
        end = 704;
    }
    tw->slideEnd = end;
    self->tweens[self->prevCursor + 1].slideDelta =
        (s16)UiAbsDiff(sprites[self->prevCursor + 1]->posX, (float)tw->slideEnd);

    /* incoming button */
    if (self->switchDir == 0) {
        ((GfxSprite **)self->base.data)[self->cursor + 1]->posX = -224.0f;
    } else {
        ((GfxSprite **)self->base.data)[self->cursor + 1]->posX = 704.0f;
    }
    sprites = (GfxSprite **)self->base.data;
    tw = &self->tweens[self->cursor + 1];
    tw->slideEnd = (s16)self->panelHomeX;
    tw->slideStart = (s16)sprites[self->cursor + 1]->posX;
    self->tweens[self->cursor + 1].slideDelta =
        (s16)UiAbsDiff(sprites[self->cursor + 1]->posX, (float)tw->slideEnd);
}
