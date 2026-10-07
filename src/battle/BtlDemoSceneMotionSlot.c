// bdc 0x0890644c BtlDemoSceneMotionSlot
#include "bdc.h"

/* Returns `slot`, or 4 for the demo ids 0x67 and 0x68. */

int BtlDemoSceneMotionSlot(BtlDemoScenePlayer *player, int slot)
{
  if (player->demoId == 0x67 || player->demoId == 0x68) {
    slot = 4;
  }
  return slot;
}
