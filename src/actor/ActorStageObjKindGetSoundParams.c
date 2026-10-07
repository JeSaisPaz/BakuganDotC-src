// bdc 0x088b7728 ActorStageObjKindGetSoundParams
#include "bdc.h"

/* Returns the 12-byte per-kind record `0x08a85608 + kind*12` (stored at `+0x13c` by
   `ActorStageObjBaseInit`; the sound/impact parameters of the kind). */

void *ActorStageObjKindGetSoundParams(int kind)

{
  return (u8 *)g_stageObjKindSoundParams + kind * 0xc;
}
