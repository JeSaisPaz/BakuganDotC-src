// bdc 0x089c2ad0 SndBgmPlayerExists
#include "bdc.h"

/* Returns 1 if streamed-audio player `index` exists: `index` is 0 or 1 and its slot in
   `g_soundBgmPlayers` is non-NULL; otherwise 0. */

bool SndBgmPlayerExists(s32 index)

{
  bool exists;
  
  exists = false;
  if (((-1 < index) && (index < 2)) && (g_soundBgmPlayers[index] != (SndBgmPlayer *)0x0)) {
    exists = true;
  }
  return exists;
}

