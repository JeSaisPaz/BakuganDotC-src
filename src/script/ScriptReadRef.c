// bdc 0x089c9e98 ScriptReadRef
#include "bdc.h"

/* Operand reader for an output operand: reads a 16-bit variable id at `script+0x30`, advances the
   operand pointer by `size` bytes (2 for a bare id, 4 when the operand is padded) and bumps the
   operand index `script+0x44`, then returns a pointer to the variable's slot: ids `< 0x1000` map to
   the local table `script+0x34 + id*4`, ids `>= 0x1000` to the global table `*0x08ac58c4 +
   (id-0x1000)*4`. */

u32 *ScriptReadRef(Script *script, int size)

{
  const u8 *p;
  u32 id;

  p = script->operand;
  id = (u32)p[0] | (u32)p[1] << 8;
  script->operand = (u8 *)p + size;
  script->operandIdx = script->operandIdx + 1;
  if ((int)id < 0x1000) {
    return script->vars + id;
  }
  return (u32 *)(g_scriptGlobalVars + ((id - 0x1000) & 0xffff));
}
