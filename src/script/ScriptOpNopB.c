// bdc 0x0880e414 ScriptOpNopB
#include "bdc.h"

/* Do-nothing opcode handler (`return 0`); reads no operands. */
int ScriptOpNopB(void *script)
{
    (void)script;
    return 0;
}
