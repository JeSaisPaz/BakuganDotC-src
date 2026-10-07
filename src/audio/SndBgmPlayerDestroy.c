// bdc 0x089c2a6c SndBgmPlayerDestroy
#include "bdc.h"

/* Destroys streamed-audio player `index` (0 or 1): when the slot of `g_soundBgmPlayers` is
   non-NULL it runs `SndBgmPlayerDestroyObj``(player, 3)` (destroys the player's lock and frees
   the object) and clears the slot; in every case it then calls the boot-thread helper
   `BootDeleteThread(index + 9)` for the player's own thread slot ("MyThread-Sound-Bgm0/1"). */

void SndBgmPlayerDestroy(s32 index)

{
  SndBgmPlayer *player;
  
  if ((-1 < index) && (index < 2)) {
    player = g_soundBgmPlayers[index];
    if (player != (SndBgmPlayer *)0x0) {
      SndBgmPlayerDestroyObj(player,3);
      g_soundBgmPlayers[index] = (SndBgmPlayer *)0x0;
    }
    BootDeleteThread(index + 9);
  }
  return;
}

