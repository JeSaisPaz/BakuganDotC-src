// bdc 0x0896c8d4 UiCardEquipHandleConfirm
#include "bdc.h"

/* Handles Cross in `UiCardEquip`: in row 0 returns 1 when the selected tab is
   enabled (`+0x2a58`) and 2 when it is not; in row 1 toggles the current card
   (`UiCardEquipToggleCard`) and returns 1, or 2 on an empty slot. Returns 0 when Cross was not
   pressed. */

int UiCardEquipHandleConfirm(UiCardEquip *self)

{
  s8 bakugan;
  s8 card;
  s32 row;
  s32 idx;

  if ((((self->base).pad)->pressed & 0x4000) != 0) {
    row = self->row;
    if (row < 1) {
      if (row >= 0) {
        if ((self->tabMask & (1 << (self->rowCursor[row] & 0x1f))) != 0) {
          return 1;
        }
        return 2;
      }
    } else if (row < 2) {
      bakugan = self->selBakugan;
      card = self->rowCursor[row];
      idx = card + bakugan * 4;
      if (self->cardIds[idx] == 0xff) {
        return 2;
      }
      if ((self->cardIds[idx + 0x10] & 1) != 0) {
        UiCardEquipToggleCard(self,0,bakugan,card);
        return 1;
      }
      UiCardEquipToggleCard(self,1,bakugan,card);
      return 1;
    }
  }
  return 0;
}
