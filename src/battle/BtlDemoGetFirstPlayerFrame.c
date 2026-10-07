// bdc 0x088ff460 BtlDemoGetFirstPlayerFrame
#include "bdc.h"

/* Returns the current scene frame (`frame`) of the battle demo's first scene player
   (`scenePlayers[0]`, `BtlDemoScenePlayer`), or 0 when there is none. */

s32 BtlDemoGetFirstPlayerFrame(BtlDemo *self)

{
  u32 frame;

  frame = 0;
  if (self->scenePlayers[0] != (BtlDemoScenePlayer *)0x0) {
    frame = self->scenePlayers[0]->frame;
  }
  return frame;
}
