// bdc 0x0892ae44 UiBakuganGetUnselectableMask
#include "bdc.h"

/* Builds the 64-bit masks of Bakugan ids that cannot be picked as partner. If save-profile byte
   `playthrough` (`+0x462`) is set, both masks stay 0. Otherwise, for ids 1..20 owned in the
   profile bitset `ownedBakugan` (`+0x50e`) whose evolved form (`UiBakuganGetPair``(0, id)`) is
   owned too, sets bit `id` in both `out[0]` and `out[1]`; then, unless profile word 0x2e
   (`SaveProfileGetWord`) is set, adds story locks to `out[0]` only, by the script stage in
   `g_scriptGlobalVars``[1]`: stage 0xd locks every id but 16, stages 8/9/10/0xe/0x25 lock ids
   1, 2 and 16, stage 0x18 locks every id but 2. */

void UiBakuganGetUnselectableMask(u32 *out)
{
  u32 masks[2];
  int id;
  int evolved;
  int i;
  s32 stage;

  memset(masks, 0, 8);
  if (SaveGetProfile()->data->playthrough == 0) {
    for (i = 1; i <= 20; i++) {
      id = (u8)i;
      if ((u8)(SaveGetProfile()->data->ownedBakugan[id / 8] & (1 << (id % 8))) == 0)
        continue;
      evolved = UiBakuganGetPair(0, id);
      if (evolved == 0)
        continue;
      if ((u8)(SaveGetProfile()->data->ownedBakugan[evolved / 8] & (1 << (evolved % 8))) == 0)
        continue;
      masks[0] |= 1u << id;
      masks[1] |= 1u << id;
    }

    if (SaveProfileGetWord(SaveGetProfile(), 0x2e) == 0) {
      stage = g_scriptGlobalVars[1];
      if (stage == 0xd) {
        for (i = 1; i <= 20; i++) {
          id = (u8)i;
          if (id != 0x10)
            masks[0] |= 1u << id;
        }
        stage = g_scriptGlobalVars[1];
      }
      switch (stage) {
      case 8:
      case 9:
      case 10:
      case 0xe:
      case 0x25:
        for (i = 1; i <= 20; i++) {
          id = (u8)i;
          if (id < 3) {
            if (id > 0)
              masks[0] |= 1u << id;
          } else if (id == 0x10) {
            masks[0] |= 1u << id;
          }
        }
        stage = g_scriptGlobalVars[1];
        break;
      default:
        break;
      }
      if (stage == 0x18) {
        for (i = 1; i <= 20; i++) {
          id = (u8)i;
          if (id != 2)
            masks[0] |= 1u << id;
        }
      }
    }
  }
  out[0] = masks[0];
  out[1] = masks[1];
}
