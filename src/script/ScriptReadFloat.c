// bdc 0x089c9dfc ScriptReadFloat
#include "bdc.h"

/* Operand reader returning a float operand: calls `ScriptReadU32` and reinterprets the result
   (`mtc1 v0,f0`; bit-copy to the FPU return register `f0`). Variable-reference operands therefore
   also yield a float from the variable's raw bits. */

float ScriptReadFloat(Script *script)
{
    u32 bits = ScriptReadU32(script);
    float value;

    __builtin_memcpy(&value, &bits, sizeof(value));
    return value;
}
