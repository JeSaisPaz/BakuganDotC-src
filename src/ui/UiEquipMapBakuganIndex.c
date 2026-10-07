// bdc 0x0895643c UiEquipMapBakuganIndex
#include "bdc.h"

/* Looks up `index` in one of two byte tables of `UiEquip`: table 0
   (`g_equipGridToBakuganId`, 20 bytes) or table 1 (`g_equipBakuganIdToGrid`, 21 bytes). The tables
   are copied to the stack first, as the original does. */

u8 UiEquipMapBakuganIndex(UiEquip *self, u8 table, u8 index)
{
  u8 gridToId[20];
  u8 idToGrid[24];

  memcpy(gridToId, g_equipGridToBakuganId, 0x14);
  memcpy(idToGrid, g_equipBakuganIdToGrid, 0x15);
  if (table == 0) {
    return gridToId[index];
  }
  return idToGrid[index];
}
