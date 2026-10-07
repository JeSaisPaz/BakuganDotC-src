// bdc 0x0880e664 ScriptOpNopC
#include "bdc.h"

/* Do-nothing opcode handler (`return 0`); reads no operands. */
int ScriptOpNopC(void *script)
{
    (void)script;
    return 0;
}
