// bdc 0x08956eb0 UiEquipPreloadMotions
#include "bdc.h"

/* Preloads the Gmo motions of the Bakugan selectable in `UiEquip`: for ids 1–20
   that are owned (profile bitset `bakuganBitsA`; all of them when `SaveGetProfileFlag0` is set)
   loads `"<select motion>.gmo"` (`UiBakuganGetSelectMotionName`, `GmoMotionLoadFile`) and,
   unless `UiEquipHasAltMotion`, also the second motion from
   `UiBakuganGetSelectTurnMotionName`. */

void UiEquipPreloadMotions(UiEquip *self)
{
  int id;
  int owned;
  char fileName[64];
  char motionName[64];

  id = 0;
  do {
    owned = 0;
    id++;
    if (SaveGetProfileFlag0() != 0) {
      owned = 1;
    } else if ((SaveGetProfile()->data->bakuganBitsA[id / 8] & (1 << (id % 8))) != 0) {
      owned = 1;
    }
    if (owned) {
      memset(motionName, 0, sizeof(motionName));
      UiBakuganGetSelectMotionName((u8)id, motionName);
      sprintf(fileName, "%s.gmo", motionName);
      GmoMotionLoadFile(GmoMotionMgrGet(), fileName);
      if (UiEquipHasAltMotion(self, 1, (u8)id) == 0) {
        memset(motionName, 0, sizeof(motionName));
        UiBakuganGetSelectTurnMotionName((u8)id, motionName);
        sprintf(fileName, "%s.gmo", motionName);
        GmoMotionLoadFile(GmoMotionMgrGet(), fileName);
      }
    }
  } while (id < 20);
}
