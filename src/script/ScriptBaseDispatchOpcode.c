// bdc 0x089c9fb4 ScriptBaseDispatchOpcode
#include "bdc.h"

/* Opcode dispatcher of the script base class (slot 2 of vtable 0x08af52a4, installed by
   ScriptBaseCtor): ignores its arguments and returns 0, i.e. every opcode is a no-op that
   advances by the instruction length. The concrete class overrides it with
   ScriptDispatchOpcode. */
int ScriptBaseDispatchOpcode(Script *script, u32 op)
{
    (void)script;
    (void)op;
    return 0;
}
