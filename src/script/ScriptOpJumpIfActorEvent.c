// bdc 0x0880e7c8 ScriptOpJumpIfActorEvent
#include "bdc.h"

/* Conditional jump on the one-shot event flag of an actor spawned by `ScriptOpSpawnActor`.
   Operands: u16 `mode`, ref (actor, validated with `ActorListContains`), u16 (discarded), u16
   `target` (only read when the actor is valid). Mode 0 jumps when `ActorTakeEventFlag` reports
   the actor was triggered; mode 1 jumps when it was triggered and its tag (`spawnTag` low byte) is
   not yet among the 10 tag bytes stored for the current stage
   (`stageActorTags[stage]`, stage = script global 1; see `ScriptOpStoreActorTagForStage`).
   Other modes never jump. A jump sets the track pc to `target` and returns 3; otherwise 0. */

int ScriptOpJumpIfActorEvent(Script *script)
{
  s32 mode;
  u32 *ref;
  void *actorRef;
  Actor *actor;
  u32 target;
  int jump;
  int found;
  int i;
  u8 tag;
  u8 *tags;
  SaveProfile *profile;

  mode = (s32)ScriptReadU16(script);
  ref = ScriptReadRef(script, 2);
  actorRef = (void *)(uintptr_t)*ref;
  ScriptReadU16(script);
  actor = (Actor *)ActorListContains(actorRef);
  jump = 0;
  if (actor == NULL) {
    return 0;
  }
  target = ScriptReadU16(script);
  if (mode == 0) {
    if (ActorTakeEventFlag(actor) != 0) {
      jump = 1;
    }
  } else if (mode == 1) {
    if (ActorTakeEventFlag(actor) != 0) {
      profile = SaveGetProfile();
      tags = profile->data->stageActorTags[g_scriptGlobalVars[1]];
      tag = (u8)actor->spawnTag;
      found = 0;
      for (i = 0; i < 10; i++) {
        if (tags[i] == tag) {
          found = 1;
          break;
        }
      }
      if (!found) {
        jump = 1;
      }
    }
  }
  if (jump) {
    script->curTrack->pc = (u16)target;
    return 3;
  }
  return 0;
}
