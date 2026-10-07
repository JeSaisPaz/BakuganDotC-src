// bdc 0x088d5fd4 GameGimmickCollectionBoxMotionName
#include "bdc.h"

/* Copies the motion name `"menu_itembox_Open"` (`which` 0) or `"menu_itembox_Close"` (1) into the
   64-byte buffer `out`. */

void GameGimmickCollectionBoxMotionName(GameGimmickCollectionBox *obj, u8 which, char *out)

{
  char buf[64];
  const char *names[2];
  u32 i;

  names[0] = "menu_itembox_Open";
  names[1] = "menu_itembox_Close";
  sprintf(buf, names[which]);
  for (i = 0; i < 0x40; i++) {
    out[i] = buf[i];
  }
}
