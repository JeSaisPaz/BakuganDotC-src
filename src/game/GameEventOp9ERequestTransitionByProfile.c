// bdc 0x088f1534 GameEventOp9ERequestTransitionByProfile
#include "bdc.h"

/* Handler of event opcode 0x9e (`GameEvent470ExecCommand`): in skip mode (flags bit 0) clears
   the fade-in-at-end flag (bit 2), then requests field transition 3 when the profile owns all of
   Bakugan 4 and 0x10..0x14, else transition 6 (`g_gameEventTransitionKind`). `flag` and `arg`
   are unused. */

void GameEventOp9ERequestTransitionByProfile(GameEvent *self, u8 flag, s16 arg)
{
  s32 ids[6];
  s32 i;

  if ((self->flags & 1) != 0) {
    self->flags = self->flags & 0xfb;
  }
  ids[0] = 4;
  ids[1] = 0x10;
  ids[2] = 0x11;
  ids[3] = 0x12;
  ids[4] = 0x13;
  ids[5] = 0x14;
  for (i = 0; i < 6; i++) {
    SaveProfile *profile = SaveGetProfile();
    s32 id = ids[i];
    if ((u8)(profile->data->ownedBakugan[id / 8] & (1 << (id % 8))) == 0) {
      break;
    }
  }
  if (i != 6) {
    g_gameEventTransitionKind = 6;
  } else {
    g_gameEventTransitionKind = 3;
  }
}
