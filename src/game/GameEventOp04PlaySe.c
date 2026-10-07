// bdc 0x088ebd14 GameEventOp04PlaySe
#include "bdc.h"

/* Handler of event opcode 0x04 (`GameEventExecCommand`): unless skipping, plays the sound effect
   mapped to `arg` by the 21-entry table `g_gameEventSeTable` (`{id, soundId}` pairs) through the sound
   manager. */

void GameEventOp04PlaySe(GameEvent *self, u8 flag, s16 arg)

{
  u32 *p;
  int i;

  if ((self->flags & 1) == 0) {
    p = g_gameEventSeTable + 40;
    for (i = 20; i >= 0; i--, p -= 2) {
      if (p[0] == (u16)arg) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), p[1], 0, 0);
        }
        return;
      }
    }
  }
}
