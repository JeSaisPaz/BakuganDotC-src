// bdc 0x088c0eb4 GameFieldCheckPlayerCaught
#include "bdc.h"

/* When the player (`ActorFindPlayer`) has been caught (`+0x354`, set by the IR sensor gimmick,
   `GameGimmickIrSensorBeamState`), handles it (`GameFieldHandlePlayerCaught` with slot
   `player+0x540`), records the slot in `+0x79b`, plays the alarm sound `0x2c0005a` and starts the
   looping sound `0x2c00023` (handle `+0x6f4`). Returns 1 when caught. */

typedef struct {
  u8 pad0[0x6f4];
  u32 loopHandle;
  u8 pad1[0x79b - 0x6f8];
  u8 caughtSlot;
} GameFieldCaughtTask;

typedef struct {
  u8 pad[0x540];
  u8 slot;
} GameFieldCaughtPlayer;

s32 GameFieldCheckPlayerCaught(CoreTask *task)

{
  GameFieldCaughtTask *t = (GameFieldCaughtTask *)task;
  Actor *player;
  u8 slot;

  player = (Actor *)ActorFindPlayer();
  if (player != NULL && player->detected != 0) {
    slot = ((GameFieldCaughtPlayer *)player)->slot;
    GameFieldHandlePlayerCaught(task, slot);
    t->caughtSlot = slot;
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x2c0005a, 0, 0);
    }
    if (SndHasManager()) {
      t->loopHandle = SndManagerPlay(SndGetManager(), 0x2c00023, 0, 0);
    }
    return 1;
  }
  return 0;
}
