// bdc 0x0889b2f8 BtlAiComScriptAddRef
#include "bdc.h"

/* Takes a reference on a CPU-AI rule script record (0x10 bytes: refcount byte `+0`, header bytes,
   group count s16 `+8`, group table `+0xc`, filled by `BtlAiLoadComScript`): loads
   `ComScript<id>.bin` on the first reference (`BtlAiLoadComScript`) and increments the refcount
   byte. */
void BtlAiComScriptAddRef(u8 *rules, s32 id)
{
    if (rules[0] == 0) {
        BtlAiLoadComScript(rules, id);
    }
    rules[0]++;
}
