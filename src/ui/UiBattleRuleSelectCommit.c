// bdc 0x08952aa8 UiBattleRuleSelectCommit
#include "bdc.h"

/* Applies the choice made on the battle-rule menu (task 340, `UiBattleRuleSelectCtor`; four
   battle types, reached from the battle-mode screen 350; the class purpose is inferred from what it
   writes to the save profile): unless `cancelled`, stores the battle type in profile word 7
   (entries 0/1 → 0, 2 → 1, 3 → 2), the sub-option of entry 3 in words 0x15/0x16
   (value, value+2; otherwise 0/2), clears words 3..6 to -1 (0 for 3/4 when
   `SaveGetProfileFlag0` is set) and resets the four slot records of the profile's battle block
   directly in the word table (words 54+2i+j and 62+2i+j = 0xff, word 70+i = 100); then sets the
   menu result (entry 0 → 1, 1 → 4, 2 → 3, 3 → 2; an out-of-range cursor sets neither word 7 nor
   a result). Cancelled: word 7 = 0, menu result 0. */

void UiBattleRuleSelectCommit(UiBattleRuleSelect *self)

{
  s8 cursor;
  u32 *words;
  s32 i;
  s32 j;

  if (self->cancelled != 0) {
    SaveProfileSetWord(SaveGetProfile(), 7, 0);
    UiSetMenuResult(&self->base, 0);
    return;
  }
  SaveProfileSetWord(SaveGetProfile(), 0x15, 0);
  SaveProfileSetWord(SaveGetProfile(), 0x16, 2);
  cursor = self->cursor;
  if (cursor < 2) {
    if (cursor >= 0) {
      if (cursor <= 0) {
        SaveProfileSetWord(SaveGetProfile(), 7, 0);
        UiSetMenuResult(&self->base, 1);
      } else {
        SaveProfileSetWord(SaveGetProfile(), 7, 0);
        UiSetMenuResult(&self->base, 4);
      }
    }
  } else if (cursor < 3) {
    SaveProfileSetWord(SaveGetProfile(), 7, 1);
    UiSetMenuResult(&self->base, 3);
  } else if (cursor < 4) {
    SaveProfileSetWord(SaveGetProfile(), 7, 2);
    UiSetMenuResult(&self->base, 2);
    SaveProfileSetWord(SaveGetProfile(), 0x15, (s32)self->subCursor);
    SaveProfileSetWord(SaveGetProfile(), 0x16, (s32)self->subCursor + 2);
  }
  SaveProfileSetWord(SaveGetProfile(), 3, 0xffffffff);
  SaveProfileSetWord(SaveGetProfile(), 4, 0xffffffff);
  SaveProfileSetWord(SaveGetProfile(), 5, 0xffffffff);
  SaveProfileSetWord(SaveGetProfile(), 6, 0xffffffff);
  if (SaveGetProfileFlag0() != 0) {
    SaveProfileSetWord(SaveGetProfile(), 3, 0);
    SaveProfileSetWord(SaveGetProfile(), 4, 0);
  }
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 2; j++) {
      words = SaveGetProfile()->words;
      if (words != NULL) {
        words[62 + i * 2 + j] = 0xff;
      }
    }
    for (j = 0; j < 2; j++) {
      words = SaveGetProfile()->words;
      if (words != NULL) {
        words[54 + i * 2 + j] = 0xff;
      }
    }
    words = SaveGetProfile()->words;
    if (words != NULL) {
      words[70 + i] = 100;
    }
  }
}
