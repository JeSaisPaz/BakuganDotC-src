// bdc 0x08928db8 UiHologramViewLoadKindTable
#include "bdc.h"

/* Copies the 16 `s16` page/message ids of view kind `kind` from `g_hologramViewKindTable` (0x20
   bytes per kind) into `kindTable` of the hologram detail view (`UiHologramViewCtor`, task 392;
   view kind `+0x485`). The whole table is first copied to the stack. */

void UiHologramViewLoadKindTable(UiHologramView *self, u32 kind)
{
  s16 table[10][16];
  s16 *dst;
  u32 k;
  int i;

  k = kind & 0xff;
  memcpy(table, g_hologramViewKindTable, 0x140);
  dst = (s16 *)self->kindTable;
  for (i = 0; i < 16; i++) {
    dst[i] = table[k][i];
  }
}
