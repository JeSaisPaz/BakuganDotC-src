// bdc 0x089c298c SndBgmPlayerCreate
#include "bdc.h"

/* Creates streamed-audio player `index` (only 0 or 1, and only when its slot in
   `g_soundBgmPlayers` is still empty): allocates the 0xd4-byte `SndBgmPlayer` from the low
   heap, constructs it with `SndBgmPlayerInit`, stores it in the slot, sets `channel = index` and
   starts the player's game thread with `BootStartThread``(index + 9, &index, 4)` (slots 9 and 10
   of `g_threadTable`, "MyThread-Sound-Bgm0/1"); the thread entry reads the 4-byte argument back
   and runs `SndBgmPlayerThreadStep` in a loop. */

void SndBgmPlayerCreate(s32 index)

{
  bool fromLow;
  SndBgmPlayer *player;
  SndBgmPlayer *slot;
  s32 arg;
  
  if (((-1 < index) && (index < 2)) && (g_soundBgmPlayers[index] == (SndBgmPlayer *)0x0)) {
    slot = (SndBgmPlayer *)0x0;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    player = MemAlloc(sizeof(SndBgmPlayer),(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (player != (SndBgmPlayer *)0x0) {
      SndBgmPlayerInit(player);
      slot = player;
    }
    g_soundBgmPlayers[index] = slot;
    slot->channel = index;
    memset(&arg,0,4);
    arg = index;
    BootStartThread(index + 9,&arg,4);
  }
  return;
}

