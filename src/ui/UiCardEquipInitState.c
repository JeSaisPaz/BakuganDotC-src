// bdc 0x08969128 UiCardEquipInitState
#include "bdc.h"

/* Initialises the state of the UiCardEquip ability-card loadout sub-screen (task 303,
   `UiCardEquipCtor`) from its constructor: `bakuganCount` = profile word 0x16
   (`SaveProfileGetWord`), `bakuganIds[0..3]` = profile words 3..6; tab cursor
   `rowCursor[0]`/`rowCursor[2]` = 2 with more than two slots else 1, `row`/`arrowRow`/`selBakugan`
   = 0; the two tab-id lists `tabIds` and `tabIdsAlt` per layout and mode (`SaveGetProfileFlag0`
   = network: two slots {1} vs {2}; four slots offline {2,3,4}, network {1,5,6} / {1,2,3});
   clears the fade record (`fadeOn`, 16 bytes) and the arrow record (`arrowAnim`, 20 bytes);
   builds the card lists (`UiCardEquipBuildCardLists`); `tabCount` = 6 (four slots) or 4. */

void UiCardEquipInitState(UiCardEquip *self)
{
  u32 i;

  self->bakuganCount = (s8)SaveProfileGetWord(SaveGetProfile(), 0x16);
  for (i = 0; i < 4; i++) {
    self->bakuganIds[i] = (s8)SaveProfileGetWord(SaveGetProfile(), i + 3);
  }
  memset(self->rowCursor, 0, 2);
  self->rowCursor[0] = (self->bakuganCount < 3) ? 1 : 2;
  self->row = 0;
  self->rowCursor[2] = self->rowCursor[0];
  self->arrowRow = 0;
  self->selBakugan = 0;

  memset(self->tabIds, 0, 4);
  if (self->bakuganCount < 3) {
    self->tabIds[1] = SaveGetProfileFlag0() ? 1 : 2;
  } else if (SaveGetProfileFlag0()) {
    self->tabIds[1] = 1;
    self->tabIds[2] = 5;
    self->tabIds[3] = 6;
  } else {
    self->tabIds[1] = 2;
    self->tabIds[2] = 3;
    self->tabIds[3] = 4;
  }

  memset(self->tabIdsAlt, 0, 4);
  if (self->bakuganCount < 3) {
    self->tabIdsAlt[1] = SaveGetProfileFlag0() ? 1 : 2;
  } else if (SaveGetProfileFlag0()) {
    self->tabIdsAlt[1] = 1;
    self->tabIdsAlt[2] = 2;
    self->tabIdsAlt[3] = 3;
  } else {
    self->tabIdsAlt[1] = 2;
    self->tabIdsAlt[2] = 3;
    self->tabIdsAlt[3] = 4;
  }

  memset(&self->fadeOn, 0, 0x10);
  UiCardEquipBuildCardLists(self);
  self->tabCount = (self->bakuganCount < 3) ? 4 : 6;
  memset(self->arrowAnim, 0, 0x14);
}
