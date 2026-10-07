// bdc 0x08a00870 ScriptOpFrameSkip
#include "bdc.h"

/* Script opcode 0x48 (group 0x40): get/set the display frame-skip interval
   `g_gfxDisplay->frameSkip`. Operands u32 `sel`, u32 `mode`, u32 `value`, then an output ref
   (`ScriptReadRef`). sel must be 0; mode 0 sets `frameSkip = value`; mode 1 reads it into the
   output variable; anything else returns 0, a non-zero sel returns 2. */

int ScriptOpFrameSkip(Script *script) {
    int ret = 2;
    u32 sel = ScriptReadU32(script);
    s32 mode = ScriptReadU32(script);
    u32 value = ScriptReadU32(script);
    u32 *out = ScriptReadRef(script, 2);

    if (sel == 0) {
        if (mode < 1) {
            if (mode < 0) {
                return 0;
            }
            g_gfxDisplay->frameSkip = value;
        } else {
            if (mode > 1) {
                return 0;
            }
            if (out == NULL) {
                return 0;
            }
            *out = g_gfxDisplay->frameSkip;
        }
        ret = 0;
    }
    return ret;
}
