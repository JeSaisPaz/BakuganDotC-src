// bdc 0x089496a0 UiBattleRecordSetModeCell
#include "bdc.h"

/* Writes one number of a row of the per-mode table of the battle-record screen (task 3005,
   `UiBattleRecordCtor`; per-Bakugan win/loss statistics from the save profile, layout package
   `"data/2d/%s/record.lzs"`): column 0 = wins (`wins[b]`), 1 = losses (`losses[b]`), 2 =
   draws (`draws[b]`), 3 = win rate in percent (wins*100/total, rounded up, 1 if non-zero), for
   Bakugan `b` in display row `row`. The digits are layout sprites (`base.data` sprite list) laid
   out right to left from index `row*16 + 0x36/0x3a/0x3e/0x41` (ones digit) on a 5-column digit
   sheet, positioned relative to the leftmost sprite of the cell (4 sprites for columns 0-2, 3 for
   the percent column); unused leading digits are hidden. A column outside 0..3 writes 0 into the
   sprites at `row*16 + 0x33`. */

#define SPR(i) (((GfxSprite **)self->base.data)[i])

static void SetDigit(GfxSprite *sprite, s32 digit)
{
  GfxSpriteSetCell(sprite, (float)(digit / 5), (float)(digit % 5));
}

void UiBattleRecordSetModeCell(UiBattleRecord *self, s32 row, s32 bakugan, s32 column)
{
  s32 idx;
  s32 value;
  s32 wins;
  s32 total;

  idx = row * 16;
  value = 0;
  if (column < 2) {
    if (column < 0) {
      idx += 0x33;
    } else if (column < 1) {
      value = self->wins[bakugan];
      idx += 0x36;
    } else {
      value = self->losses[bakugan];
      idx += 0x3a;
    }
  } else if (column < 3) {
    idx += 0x3e;
    value = self->draws[bakugan];
  } else if (column > 3) {
    idx += 0x33;
  } else {
    wins = self->wins[bakugan];
    idx += 0x41;
    if (wins == 0) {
      value = 0;
    } else {
      total = wins + self->losses[bakugan] + self->draws[bakugan];
      value = (wins * 100) / total;
      if (!((float)(wins * 100) / (float)total <= (float)value)) {
        value = value + 1;
      } else if (value == 0) {
        value = 1;
      }
    }
  }

  if (column == 3) {
    /* sprites idx (ones), idx-1 (tens), idx-2 (hundreds; also the x anchor) */
    if (value < 10) {
      SetDigit(SPR(idx), value);
      SPR(idx)->posX = SPR(idx - 2)->posX + 8.0f;
      SPR(idx)->flags |= 1;
      SPR(idx - 1)->flags &= ~1u;
      SPR(idx - 2)->flags &= ~1u;
    } else if (value < 100) {
      SetDigit(SPR(idx), value % 10);
      SPR(idx)->posX = SPR(idx - 2)->posX + 12.0f;
      SetDigit(SPR(idx - 1), value / 10);
      SPR(idx - 1)->posX = SPR(idx - 2)->posX + 4.0f;
      SPR(idx)->flags |= 1;
      SPR(idx - 1)->flags |= 1;
      SPR(idx - 2)->flags &= ~1u;
    } else {
      SetDigit(SPR(idx), value % 10);
      SPR(idx)->posX = SPR(idx - 2)->posX + 16.0f;
      SetDigit(SPR(idx - 1), (value % 100) / 10);
      SPR(idx - 1)->posX = SPR(idx - 2)->posX + 8.0f;
      SetDigit(SPR(idx - 2), value / 100);
      SPR(idx)->flags |= 1;
      SPR(idx - 1)->flags |= 1;
      SPR(idx - 2)->flags |= 1;
    }
  } else {
    /* sprites idx (ones) .. idx-3 (thousands; also the x anchor) */
    if (value < 10) {
      SetDigit(SPR(idx), value);
      SPR(idx)->posX = SPR(idx - 3)->posX + 12.0f;
      SPR(idx)->flags |= 1;
      SPR(idx - 1)->flags &= ~1u;
      SPR(idx - 2)->flags &= ~1u;
      SPR(idx - 3)->flags &= ~1u;
    } else if (value < 100) {
      SetDigit(SPR(idx), value % 10);
      SPR(idx)->posX = SPR(idx - 3)->posX + 16.0f;
      SetDigit(SPR(idx - 1), value / 10);
      SPR(idx - 1)->posX = SPR(idx - 3)->posX + 8.0f;
      SPR(idx)->flags |= 1;
      SPR(idx - 1)->flags |= 1;
      SPR(idx - 2)->flags &= ~1u;
      SPR(idx - 3)->flags &= ~1u;
    } else if (value < 1000) {
      SetDigit(SPR(idx), value % 10);
      SPR(idx)->posX = SPR(idx - 3)->posX + 20.0f;
      SetDigit(SPR(idx - 1), (value % 100) / 10);
      SPR(idx - 1)->posX = SPR(idx - 3)->posX + 12.0f;
      SetDigit(SPR(idx - 2), value / 100);
      SPR(idx - 2)->posX = SPR(idx - 3)->posX + 4.0f;
      SPR(idx)->flags |= 1;
      SPR(idx - 1)->flags |= 1;
      SPR(idx - 2)->flags |= 1;
      SPR(idx - 3)->flags &= ~1u;
    } else {
      SetDigit(SPR(idx), value % 10);
      SPR(idx)->posX = SPR(idx - 3)->posX + 24.0f;
      SetDigit(SPR(idx - 1), (value % 100) / 10);
      SPR(idx - 1)->posX = SPR(idx - 3)->posX + 16.0f;
      SetDigit(SPR(idx - 2), (value % 1000) / 100);
      SPR(idx - 2)->posX = SPR(idx - 3)->posX + 8.0f;
      SetDigit(SPR(idx - 3), value / 1000);
      SPR(idx)->flags |= 1;
      SPR(idx - 1)->flags |= 1;
      SPR(idx - 2)->flags |= 1;
      SPR(idx - 3)->flags |= 1;
    }
  }
}

#undef SPR
