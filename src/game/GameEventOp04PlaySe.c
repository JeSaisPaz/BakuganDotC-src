// bdc 0x088ebd14 GameEventOp04PlaySe
#include "bdc.h"

/* Handler of event opcode 0x04 (`GameEventExecCommand`): unless skipping, plays the sound effect
   mapped to `id` by the `{id, soundId}` pairs of `g_gameEventSeTable` through the sound manager.

   PSP: the original walks 21 pairs down from `g_gameEventSeTable + 40` (0x08a99078), but only the
   first 5 pairs are the table; pairs 5..20 are its neighbours: the message format strings,
   `g_gameEventFixedHalf` (pair 6), `g_gameEventFixedScale` (pair 10) and `g_gameEventVoiceIds`
   (pairs 11..20 = voice ids [0..19]). Of those words only the voice ids and the low word of
   g_gameEventFixedHalf (0x800, paired with its high word 0) fit a u16 id, so the host reads those
   by name, in the PSP's top-down order, instead of running off the 10-word table. */

void GameEventOp04PlaySe(GameEvent *self, u16 id, s16 unused)

{
  const u32 *p;
  u32 sound;
  int i;

  if ((self->flags & 1) != 0) {
    return;
  }
  for (i = 18; i >= 0; i -= 2) {
    p = &g_gameEventVoiceIds[i];
    if (p[0] == id) {
      sound = p[1];
      goto play;
    }
  }
  if ((u32)g_gameEventFixedHalf == id) {
    sound = (u32)((u64)g_gameEventFixedHalf >> 32);
    goto play;
  }
  for (i = 8; i >= 0; i -= 2) {
    p = &g_gameEventSeTable[i];
    if (p[0] == id) {
      sound = p[1];
      goto play;
    }
  }
  return;

play:
  if (SndHasManager()) {
    SndManagerPlay(SndGetManager(), sound, 0, 0);
  }
}
