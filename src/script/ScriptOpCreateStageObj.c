// bdc 0x088107bc ScriptOpCreateStageObj
#include "bdc.h"

/* Script opcode: creates a stage object (scenery prop with HP, `ActorStageObjCreate`, 0x340
   bytes) of kind `id/6 * ... + id%6` from the table at `0x08a348d0`, at a 4-float position/heading,
   and stores the new object through the first operand's variable slot. */

int ScriptOpCreateStageObj(Script *script) {
    u32 *out = ScriptReadRef(script, 2);
    s32 id = ScriptReadU32(script);
    float pos[4] __attribute__((aligned(16)));
    u32 instanceId;

    pos[3] = 0.0f;
    pos[2] = 0.0f;
    pos[1] = 0.0f;
    pos[0] = 0.0f;
    pos[0] = ScriptReadFloat(script);
    pos[1] = ScriptReadFloat(script);
    pos[2] = ScriptReadFloat(script);
    pos[3] = ScriptReadFloat(script);
    instanceId = ScriptReadU32(script);
    *out = (u32)(uintptr_t)ActorStageObjCreate(g_stageObjKindTable[id / 6] + id % 6, pos, instanceId); /* PSP: 32-bit script variable holds a pointer; port handle needed */
    return 0;
}
