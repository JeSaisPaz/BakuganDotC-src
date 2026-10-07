// bdc 0x0894b8b8 UiBattleRecordStartTotalScroll
#include "bdc.h"

/* Starts a one-row scroll of the totals table of the battle-record screen
   (`UiBattleRecord`); counterpart of `UiBattleRecordStartModeScroll`.
   For `scrollDir` 1 (up) it sets bit 0 of sprite 9's flags, fills the spare row 5 with Bakugan
   `scrollTop + 4`, places it at row 4's Y with alpha 1 and hides row 0 (alpha 0), then refills
   rows 0..4 with `scrollTop - 1 + row` and moves them up by 23 px. For `scrollDir` 2 (down) it
   sets bit 0 of sprite 10, fills row 5 with `scrollTop`, places it at row 0's Y (alpha 1),
   hides row 4, refills rows 0..4 with `scrollTop + 1 + row` and moves them down by 23 px.
   Table sprites are 4 column-0 sprites per row plus one per row in columns 1..7
   (`g_uiBattleRecordTotalColumnSprites`); `UiBattleRecordUpdateTotalScroll` then slides
   them back. Always resets `scrollFrame`. */

#define VISIBLE_ROWS  5
#define SPARE_ROW     5
#define COLUMN_COUNT  8
#define CELL_COUNT    4     /* sprites per row in column 0 */
#define ROW_STEP      23.0f

#define SPRITE(self, index)      (((GfxSprite **)(self)->base.data)[index])

/* Moves the spare row onto row `fromRow` (alpha 1) and hides row `hideRow` (alpha 0). */
static void PlaceSpareRow(UiBattleRecord *self, s32 fromRow, s32 hideRow)
{
    s32 cell;
    s32 col;

    for (cell = 0; cell < CELL_COUNT; cell++) {
        SPRITE(self, g_uiBattleRecordTotalColumnSprites[0] + cell + SPARE_ROW * CELL_COUNT)->posY =
            SPRITE(self, g_uiBattleRecordTotalColumnSprites[0] + cell + fromRow * CELL_COUNT)->posY;
        SPRITE(self, g_uiBattleRecordTotalColumnSprites[0] + cell + SPARE_ROW * CELL_COUNT)->alpha = 1.0f;
        SPRITE(self, g_uiBattleRecordTotalColumnSprites[0] + cell + hideRow * CELL_COUNT)->alpha = 0.0f;
    }
    for (col = 1; col < COLUMN_COUNT; col++) {
        SPRITE(self, g_uiBattleRecordTotalColumnSprites[col] + SPARE_ROW)->posY =
            SPRITE(self, g_uiBattleRecordTotalColumnSprites[col] + fromRow)->posY;
        SPRITE(self, g_uiBattleRecordTotalColumnSprites[col] + SPARE_ROW)->alpha = 1.0f;
        SPRITE(self, g_uiBattleRecordTotalColumnSprites[col] + hideRow)->alpha = 0.0f;
    }
}

/* Refills rows 0..4 with Bakugan `scrollTop + first + row` and moves them by `dy` in Y. */
static void RefillRows(UiBattleRecord *self, s32 first, float dy)
{
    s32 row;
    s32 cell;
    s32 col;

    for (row = 0; row < VISIBLE_ROWS; row++) {
        UiBattleRecordSetTotalRow(self, row, self->scrollTop + row + first);
        for (cell = 0; cell < CELL_COUNT; cell++) {
            SPRITE(self, g_uiBattleRecordTotalColumnSprites[0] + cell + row * CELL_COUNT)->posY += dy;
        }
        for (col = 1; col < COLUMN_COUNT; col++) {
            SPRITE(self, g_uiBattleRecordTotalColumnSprites[col] + row)->posY += dy;
        }
    }
}

void UiBattleRecordStartTotalScroll(UiBattleRecord *self)
{
    if (self->scrollDir == 1) {
        SPRITE(self, 9)->flags |= 1;
        UiBattleRecordSetTotalRow(self, SPARE_ROW, self->scrollTop + 4);
        PlaceSpareRow(self, 4, 0);
        RefillRows(self, -1, -ROW_STEP);
    } else if (self->scrollDir == 2) {
        SPRITE(self, 10)->flags |= 1;
        UiBattleRecordSetTotalRow(self, SPARE_ROW, self->scrollTop);
        PlaceSpareRow(self, 0, 4);
        RefillRows(self, 1, ROW_STEP);
    }
    self->scrollFrame = 0;
}
