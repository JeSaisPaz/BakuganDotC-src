// bdc 0x0880e66c ScriptOpSpawnActor
#include "bdc.h"

/* Script opcode that spawns an in-world 3D actor with `ActorSpawn``(modelId, flag, &pos)` and
   writes it to the output ref. Operands: output ref, u32 `base`, u32 `flag`, 4 floats (x, y, z,
   heading), u32 `tag`. `modelId = base + 0x23` (`base + 0x26` for ids above 0x2f when the current
   stage, script global 1, is 8..11); model 0x2f is only spawned when profile word 9 equals `tag`
   (otherwise nothing is spawned and the ref is left alone). The tag is stored at actor `+0x320`
   (read back by `ScriptOpStoreActorTagForStage`, `ScriptOpIsActorTagStoredForStage` and
   `ScriptOpJumpIfActorEvent`). Returns 0. */

int ScriptOpSpawnActor(Script *script)

{
  u32 *out;
  u32 base;
  u32 flag;
  u32 tag;
  s32 modelId;
  Actor *actor;
  float pos[4] __attribute__((aligned(16)));

  /* script variables are 32-bit slots; on the PSP they hold the actor pointer */
  out = ScriptReadRef(script, 2);
  base = ScriptReadU32(script);
  modelId = base + 0x23;
  flag = ScriptReadU32(script);
  pos[2] = 0.0f;
  pos[1] = 0.0f;
  pos[0] = 0.0f;
  pos[3] = 0.0f;
  pos[0] = ScriptReadFloat(script);
  pos[1] = ScriptReadFloat(script);
  pos[2] = ScriptReadFloat(script);
  pos[3] = ScriptReadFloat(script);
  tag = ScriptReadU32(script);
  if (modelId == 0x2f) {
    if (tag != SaveProfileGetWord(SaveGetProfile(), 9)) {
      return 0;
    }
  }
  if (modelId > 0x2f) {
    switch (g_scriptGlobalVars[1]) {
    case 8:
    case 9:
    case 10:
    case 11:
      modelId = base + 0x26;
      break;
    default: /* 0..7 and anything else keep base + 0x23 */
      break;
    }
  }
  actor = (Actor *)ActorSpawn(modelId, flag, pos);
  *out = PspAddr(actor);
  actor->spawnTag = tag;
  return 0;
}
