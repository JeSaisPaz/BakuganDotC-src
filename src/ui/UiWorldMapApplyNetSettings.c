// bdc 0x0899edfc UiWorldMapApplyNetSettings
#include "bdc.h"

/* Applies the settings part of a network state packet from player `slot` to
   `UiWorldMap`: for the host (slot 0) in a network session writes changed values
   back to profile words 0x19 (index 0/1/2 → 100/200/300, else -1), 0x1a, and 0x1b (1/3/5 → 0/1/2,
   other values as is); then copies the four packet shorts over the player record's settings
   block (`+0x8..+0xf`). Out-of-range slots return without doing anything. */

void UiWorldMapApplyNetSettings(UiScreen *screen, int slot, s8 *data)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  const s16 *words = (const s16 *)data;
  s16 *dst;
  s16 w0, w1, w2, w3;
  s32 value;

  if (slot < 0 || slot >= 4) {
    return;
  }
  if (map->netSession != 0 && slot == 0) {
    if (map->player[slot].setting[0] != data[0]) {
      if (data[0] <= 0) {
        value = -1;
        if (data[0] >= 0) {
          value = 100;
        }
      } else if (data[0] < 2) {
        value = 200;
      } else {
        value = -1;
        if (data[0] < 3) {
          value = 300;
        }
      }
      SaveProfileSetWord(SaveGetProfile(), 0x19, (u32)value);
    }
    if (map->player[slot].setting[1] != data[1]) {
      SaveProfileSetWord(SaveGetProfile(), 0x1a, (u32)(s32)data[1]);
    }
    if (map->player[slot].setting[2] != data[2]) {
      value = data[2];
      if (value == 5) {
        value = 2;
      } else if (value == 3) {
        value = 1;
      } else if (value == 1) {
        value = 0;
      }
      SaveProfileSetWord(SaveGetProfile(), 0x1b, (u32)value);
    }
  }
  dst = (s16 *)map->player[slot].setting;
  w0 = words[0];
  w1 = words[1];
  w2 = words[2];
  dst[0] = w0;
  w3 = words[3];
  dst[1] = w1;
  dst[2] = w2;
  dst[3] = w3;
}
