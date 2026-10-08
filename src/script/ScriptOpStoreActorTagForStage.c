// bdc 0x0880ea9c ScriptOpStoreActorTagForStage
#include "bdc.h"

/* Script opcode: reads an actor ref and a u16 `slot`; if the actor is valid (`ActorListContains`)
   stores the low byte of its `spawnTag` (set by `ScriptOpSpawnActor`) into the per-stage tag list
   of the profile, `stageActorTags[stage][slot & 0xff]` (stage = script global 1). Returns 0. */

int ScriptOpStoreActorTagForStage(Script *script)
{
  u32 *ref;
  void *actorRef;
  u8 slot;
  Actor *actor;
  SaveProfile *profile;

  ref = ScriptReadRef(script, 2);
  actorRef = PspPtr(*ref);
  slot = (u8)ScriptReadU16(script);
  actor = (Actor *)ActorListContains(actorRef);
  if (actor != NULL) {
    profile = SaveGetProfile();
    profile->data->stageActorTags[g_scriptGlobalVars[1]][slot] = (u8)actor->spawnTag;
  }
  return 0;
}
