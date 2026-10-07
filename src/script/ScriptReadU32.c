// bdc 0x089c9d68 ScriptReadU32
#include "bdc.h"

/* Operand reader of the script VM: fetches the next 32-bit operand (stored as a u16 + two bytes,
   i.e. unaligned LE) at `script+0x30`, advances the pointer by 4 and bumps the operand index
   `script+0x44`. Bit `index` of the mask `script+0x42` makes the operand a variable reference: ids
   `< 0x1000` read the local table `script+0x34`, ids `>= 0x1000` read the global table
   `*0x08ac58c4` at `id-0x1000`; otherwise the immediate is returned. */

u32 ScriptReadU32(Script *script)

{
  u8 idx;
  const u8 *p;
  u32 val;

  p = script->operand;
  idx = script->operandIdx;
  val = (u32)p[2] << 0x10 | (u32)p[3] << 0x18 | ((u32)p[0] | (u32)p[1] << 8);
  script->operand = (u8 *)p + 4;
  if ((script->refMask & (1 << (idx & 0x1f))) != 0) {
    if ((int)val >= 0x1000) {
      val = g_scriptGlobalVars[val - 0x1000];
      script->operandIdx = idx + 1;
      return val;
    }
    val = script->vars[val];
  }
  script->operandIdx = idx + 1;
  return val;
}
