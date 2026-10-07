// bdc 0x089ffe7c ScriptDispatchOpcode
#include "bdc.h"

/* Opcode dispatcher of the script VM: vtable slot 2 of the concrete `Script` class (vtable
   `0x08af59fc`), called by `ScriptStep` as `fn(script + delta, opcode)`. It selects an
   operand-handler table by the two top bits of the opcode byte, fetches the 8-byte `MemberFnPtr`
   entry for that opcode (GCC 2.x pointer-to-member: `this += delta`; if `index != 0` the call goes
   through the vtable slot `index` of the adjusted object), calls the handler with `this` and
   returns the handler's result (return codes: see `ScriptStep`); an opcode with an unknown group
   yields 0 (cannot happen: the four groups cover every byte). Groups 0x40 (op >= 0x60), 0x80 and
   0xc0 alias one array, `g_scriptOpTableHi`, at slots `op - 0x60`, `op - 0x7f`, `op - 0xbb`. */

static inline int ScriptCallMember(Script *script, const MemberFnPtr *member)
{
    u8 *self = (u8 *)script + member->delta;
    void *fn = member->pfn;

    if (member->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(self + (intptr_t)member->pfn);
        const VtblEntry *entry = &vtbl[member->index];

        fn = entry->fn;
        self += entry->delta;
    }
    return ((int (*)(void *))fn)(self);
}

int ScriptDispatchOpcode(Script *script, u32 op)
{
    u32 group;

    op &= 0xff;
    group = op & 0xc0;
    if (group == 0xc0) {
        return ScriptCallMember(script, &g_scriptOpTableHi[op - 0xbb]);
    }
    if (group == 0x80) {
        return ScriptCallMember(script, &g_scriptOpTableHi[op - 0x7f]);
    }
    if (group == 0x40) {
        if (op < 0x60) {
            /* Only 0x40-0x48 are valid entries of the 9-entry table. */
            return ScriptCallMember(script, (const MemberFnPtr *)g_scriptOpTable40 + (op - 0x40));
        }
        return ScriptCallMember(script, &g_scriptOpTableHi[op - 0x60]);
    }
    if (group != 0) {
        return 0;
    }
    return ScriptCallMember(script, &g_scriptOpTable00[op]);
}
