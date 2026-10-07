// bdc 0x08a1ff8c SndSsProgramKeyOn
#include "bdc.h"

/* Sequencer key-on by program: under the layer mutex it looks up program `program` of bank `bankId`
   (`SndSsPhdGetProgram`) and keys on every tone of that program whose key range
   (`tone+0x10..+0x14`) contains `note` through `SndSsKeyOnTone`. The bank slot is re-read
   after the lock and on every tone. Returns the bit mask of the
   voices started (0 when the layer is down, the bank is missing or the program is empty). */

u32 SndSsProgramKeyOn(u32 bankId, u32 channel, u32 program, u32 note, u32 flags, void *volPan, s32 handle)

{
  u32 mask;
  u32 i;
  s32 voice;
  u32 *prog;    /* [0] tone count, [4 + i] tone index i */
  u32 *tone;    /* [4] lowest key, [5] highest key */

  mask = 0;
  if (g_sndSsState == -1 || bankId >= 0x80 || g_sndSsBankTable[bankId] == 0) {
    return 0;
  }
  sceKernelLockLwMutex(&g_sndSsMutex,1,NULL);
  if (SndSsPhdGetProgram(*(void **)(uintptr_t)g_sndSsBankTable[bankId],program,(void **)&prog) == 0 && prog[0] != 0) {
    i = 0;
    do {
      if (SndSsPhdGetTone(*(void **)(uintptr_t)g_sndSsBankTable[bankId],prog[4 + i],(void **)&tone) >= 0 &&
          note >= tone[4] && note <= tone[5]) {
        voice = SndSsKeyOnTone(bankId,prog[4 + i],channel,note,0,flags,handle,volPan);
        if (voice >= 0) {
          mask |= 1u << voice;
        }
      }
      i++;
    } while (i < prog[0]);
  }
  sceKernelUnlockLwMutex(&g_sndSsMutex,1);
  return mask;
}
