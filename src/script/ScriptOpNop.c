// bdc 0x089c9fbc ScriptOpNop
#include "bdc.h"

/* Opcode handler that does nothing: returns 0, so ScriptStep advances pc by the instruction
   length and continues with the next instruction. */
int ScriptOpNop(Script *script)
{
    (void)script;
    return 0;
}
