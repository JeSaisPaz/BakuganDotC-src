// bdc 0x088c0eb4 GameFieldCheckPlayerCaught
#include "bdc.h"

/* When the player (`ActorFindPlayer`) has been caught (`+0x354`, set by the IR sensor gimmick,
   `GameGimmickIrSensorBeamState`), handles it (`GameFieldHandlePlayerCaught` with slot
   `player+0x540`), records the slot in `+0x79b`, plays the alarm sound `0x2c0005a` and starts the
   looping sound `0x2c00023` (handle `+0x6f4`). Returns 1 when caught. */

s32 GameFieldCheckPlayerCaught(CoreTask *task)

{
  GameFieldTask *t = (GameFieldTask *)task;
  Actor *player;
  u8 slot;

  player = (Actor *)ActorFindPlayer();
  if (player != NULL && player->detected != 0) {
    slot = ((ActorPlayer *)player)->behaviourMark;
    GameFieldHandlePlayerCaught(task, slot);
    t->caughtSlot = slot;
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x2c0005a, 0, 0);
    }
    if (SndHasManager()) {
      t->alarmLoopHandle = SndManagerPlay(SndGetManager(), 0x2c00023, 0, 0);
    }
    return 1;
  }
  return 0;
}
