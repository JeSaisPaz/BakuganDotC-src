// bdc 0x088b7728 ActorStageObjKindGetSoundParams
#include "bdc.h"

/* Returns row `kind` of `g_stageObjKindSoundParams` (three s32 sound ids, `0x08a85608 + kind*12`), stored in `soundParams` by `ActorStageObjBaseInit`: the sound/impact parameters of the kind. */

const s32 *ActorStageObjKindGetSoundParams(int kind)

{
  return g_stageObjKindSoundParams[kind];
}
