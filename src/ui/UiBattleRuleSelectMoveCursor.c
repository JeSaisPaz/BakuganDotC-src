// bdc 0x089531b8 UiBattleRuleSelectMoveCursor
#include "bdc.h"

/* Moves the selection `cursor` of the battle-rule menu (task 340, `UiBattleRuleSelectCtor`; four
   battle types, reached from the battle-mode screen 350; the class purpose is inferred from what it
   writes to the save profile) one step: in the direction queued by
   `UiBattleRuleSelectAnimateCursorMove` when `switchQueued` is set (`switchDir`: 0 = next,
   1 = previous), otherwise next on pad bit 0x80 / previous on pad bit 0x20. Disabled entries
   (`buttonEnabled[i]` == 0) are skipped with wrap-around over the four entries (at most four
   steps). Remembers the old entry in `prevCursor`, clears the move state (`switchDir..switchT`),
   stores the direction in `switchDir`, sets `switchFrames` = 8.0 and returns 1.
   Returns 0 when nothing is queued and neither pad bit is down. */

s32 UiBattleRuleSelectMoveCursor(UiBattleRuleSelect *self)

{
  u8 dir;
  s8 cursor;
  s32 tries;

  if (self->switchQueued != 0) {
    dir = (self->switchDir != 0);
  } else if ((self->base.pad->buttons & 0x80) != 0) {
    dir = 0;
  } else if ((self->base.pad->buttons & 0x20) != 0) {
    dir = 1;
  } else {
    return 0;
  }
  cursor = self->cursor;
  self->prevCursor = cursor;
  tries = 0;
  if (dir == 0) {
    do {
      cursor = (s8)(cursor + 1);
      if (cursor >= 4) {
        cursor = 0;
      }
    } while (self->buttonEnabled[cursor] == 0 && ++tries < 4);
  } else {
    do {
      cursor = (s8)(cursor - 1);
      if (cursor < 0) {
        cursor = 3;
      }
    } while (self->buttonEnabled[cursor] == 0 && ++tries < 4);
  }
  self->cursor = cursor;
  memset(&self->switchDir, 0, 0xc);
  self->switchDir = dir;
  self->switchFrames = 8.0f;
  return 1;
}
