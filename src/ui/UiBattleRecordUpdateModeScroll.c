// bdc 0x0894a760 UiBattleRecordUpdateModeScroll
#include "bdc.h"

/* Advances the row scroll of the per-mode table of the battle-record screen
   (`UiBattleRecord`) by one frame. While `scrollFrame` < 2 it moves every
   table sprite (16 column-0 sprites per row and one sprite per row in columns 1..5, indices from
   `g_uiBattleRecordModeColumnSprites`) by +11.5 (`scrollDir` 1, up) or -11.5 (`scrollDir` 2,
   down) in Y, lowers row 5's alpha by 0.5 and raises row 0's (up) or row 4's (down) by 0.5, sets
   add colour 0.3 on the arrow sprite in use (7 up, 8 down), then increments `scrollFrame` and
   returns 0. Otherwise, for up: sets bit 0 of sprite 8's flags, resets sprite 7's add colour to
   0, clears bit 0 of sprite 9, decrements `scrollTop` and clears bit 0 of sprite 7 when it
   reaches <= 0; for down: sets bit 0 of sprite 7, resets sprite 8's add colour, clears bit 0 of
   sprite 10, increments `scrollTop` and clears bit 0 of sprite 8 when it reaches >= 15. In every
   direction (also 0) it then clears `scrollDir` and `scrollFrame` and returns 1. */

#define ROW_COUNT     6
#define COLUMN_COUNT  6
#define CELL_COUNT    16    /* sprites per row in column 0 */
#define ROW_STEP      11.5f

#define SPRITE(self, index)      (((GfxSprite **)(self)->base.data)[index])

static void SetAddColor(GfxSprite *sprite, float rgb)
{
    sprite->addColor[0] = rgb;
    sprite->addColor[1] = rgb;
    sprite->addColor[2] = rgb;
    sprite->addColor[3] = 1.0f;
}

/* Moves every table sprite by `dy`, fades row `fadeOutRow` out and row `fadeInRow` in by 0.5. */
static void ShiftRows(UiBattleRecord *self, float dy, s32 fadeOutRow, s32 fadeInRow)
{
    const s32 *base = g_uiBattleRecordModeColumnSprites;
    s32 row;
    s32 col;
    s32 cell;

    for (row = 0; row < ROW_COUNT; row++) {
        for (cell = 0; cell < CELL_COUNT; cell++) {
            SPRITE(self, base[0] + cell + row * CELL_COUNT)->posY += dy;
        }
        for (col = 1; col < COLUMN_COUNT; col++) {
            SPRITE(self, base[col] + row)->posY += dy;
        }
    }
    for (cell = 0; cell < CELL_COUNT; cell++) {
        SPRITE(self, base[0] + cell + fadeOutRow * CELL_COUNT)->alpha -= 0.5f;
        SPRITE(self, base[0] + cell + fadeInRow * CELL_COUNT)->alpha += 0.5f;
    }
    for (col = 1; col < COLUMN_COUNT; col++) {
        SPRITE(self, base[col] + fadeOutRow)->alpha -= 0.5f;
        SPRITE(self, base[col] + fadeInRow)->alpha += 0.5f;
    }
}

s32 UiBattleRecordUpdateModeScroll(UiBattleRecord *self)
{
    s32 direction = self->scrollDir;

    if (self->scrollFrame < 2) {
        /* Two frames of movement, highlighting the arrow being used. */
        if (direction == 1) {
            ShiftRows(self, ROW_STEP, 5, 0);
            SetAddColor(SPRITE(self, 7), 0.3f);
        } else if (direction == 2) {
            ShiftRows(self, -ROW_STEP, 5, 4);
            SetAddColor(SPRITE(self, 8), 0.3f);
        }
        self->scrollFrame++;
        return 0;
    }

    if (direction == 1) {
        SPRITE(self, 8)->flags |= 1;
        SetAddColor(SPRITE(self, 7), 0.0f);
        SPRITE(self, 9)->flags &= ~1u;
        self->scrollTop--;
        if (self->scrollTop <= 0) {
            SPRITE(self, 7)->flags &= ~1u;
        }
    } else if (direction == 2) {
        SPRITE(self, 7)->flags |= 1;
        SetAddColor(SPRITE(self, 8), 0.0f);
        SPRITE(self, 10)->flags &= ~1u;
        self->scrollTop++;
        if (self->scrollTop >= 15) {
            SPRITE(self, 8)->flags &= ~1u;
        }
    }
    self->scrollDir = 0;
    self->scrollFrame = 0;
    return 1;
}
