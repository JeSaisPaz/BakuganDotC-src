// bdc 0x089a5fec UiMainMenuItemDistance
#include "bdc.h"

/* Returns how many carousel steps separate item `from` from item `to` (5x5 byte table at
   `0x08ac3d61`: `(from - to) mod 5`). */

u8 UiMainMenuItemDistance(UiMainMenu *self, u8 from, u8 to)

{
  u8 table[28];

  memcpy(table,g_uiMainMenuItemDistTable,0x19);
  return table[from * 5 + to];
}

