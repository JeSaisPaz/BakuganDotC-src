// bdc 0x0880eb30 ScriptOpIsActorTagStoredForStage
#include "bdc.h"

/* Script opcode: two refs (actor, result). If the actor is valid (`ActorListContains`), writes 1
   to the result when its tag byte (`+0x320`) is among the 10 bytes of the current stage's tag list
   (`*profile + 0x2d0 + stage*10`, written by `ScriptOpStoreActorTagForStage`), else 0; an invalid
   actor leaves the result ref unread and untouched. Returns 0. */

int ScriptOpIsActorTagStoredForStage(Script *script)

{
  u32 *ref;
  Actor *actor;
  u32 *result;
  SaveProfile *profile;
  u8 *tags;
  u8 tag;
  int i;
  u32 found;

  ref = ScriptReadRef(script, 2);
  /* script variables are 32-bit slots; on the PSP they hold the actor pointer */
  actor = (Actor *)ActorListContains((void *)(uintptr_t)*ref);
  if (actor == NULL) {
    return 0;
  }
  result = ScriptReadRef(script, 2);
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
  *result = found;
  return 0;
}
