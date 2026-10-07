// bdc 0x08987ed0 UiCollectionTheaterMapSceneId
#include "bdc.h"

/* Maps between theater slots and scene/event ids through the 21-entry u16 table `0x08ac3a00`:
   `reverse` = 0 returns the id of slot `value` (slot 0 when out of range); otherwise returns the
   slot of id `value`, or -1. */

int UiCollectionTheaterMapSceneId(u8 reverse, u16 value)
{
  u16 table[22];
  int i;

  memcpy(table, g_theaterSceneIdTable, 0x2c);
  if (reverse == 0) {
    if (value < 0x15) {
      table[0] = table[value];
    }
    return table[0];
  }
  i = 0;
  do {
    if (table[i] == value) {
      return i;
    }
    i++;
  } while (i < 0x15);
  return -1;
}
