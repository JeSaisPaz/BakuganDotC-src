// bdc 0x089c9e54 ScriptSkipString
#include "bdc.h"

/* Skips an inline NUL-terminated string operand: advances `script->operand` past the string and its
   terminator, with a minimum of 5 bytes (so a short string still occupies the 4-byte aligned slot
   plus terminator), and bumps the operand index. Handlers that need the text read the pointer from
   `script->operand` first, then call this. */

void ScriptSkipString(Script *script)
{
    u8 *p = script->operand;
    int len = 1;

    if (*(s8 *)p != 0) {
        while (*(s8 *)(p + len) != 0) {
            len++;
        }
        len++;
    }
    if (len < 5) {
        len = 5;
    }
    script->operand = p + len;
    script->operandIdx = script->operandIdx + 1;
}
