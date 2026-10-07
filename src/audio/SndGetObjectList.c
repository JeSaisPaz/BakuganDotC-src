// bdc 0x089c2618 SndGetObjectList
#include "bdc.h"

/* Returns the list-owner object of the sound-object system (`*g_soundObjectMgr`, the 0x30-byte
   object whose `+0x24` is the head of the sound-object list). Callers: `BootEndOfFrame` (feeds it
   to `SndObjectMgrUpdate`) and about 6 script/game functions that add objects. */

void *SndGetObjectList(void)

{
  return *g_soundObjectMgr;
}

