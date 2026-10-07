// bdc 0x08810158 ScriptOpGetPlayerKnockOutCount
#include "bdc.h"

/* Script opcode that reads the player unit's knock-out count into a variable: operands u16 `sel`,
   output ref. `sel` 0 stores the talk window's copy (`(s16)task->+0x904`, task 0x6e,
   `UiGetTalkTask`; `UiTalkWindowUpdate` adds every change of the unit's `+0x580` to it and
   `UiTalkShowMessage` can count it down); `sel` 1 stores the player unit's `+0x580` directly
   (`BtlGetPlayerBakugan`, `BtlBakuganListFind`). Other values store nothing. Returns 0. */

int ScriptOpGetPlayerKnockOutCount(Script *script) {
    s16 sel = (s16)ScriptReadU16(script);
    u32 *out = ScriptReadRef(script, 2);
    BtlBakugan *unit = BtlBakuganListFind(BtlGetPlayerBakugan());

    if (sel < 1) {
        if (sel >= 0 && UiTalkTaskExists() != 0) {
            *out = (s16)UiGetTalkTask()->talkUnitDelta;
        }
    } else if (sel < 2 && unit != NULL) {
        *out = unit->knockoutCount;
    }
    return 0;
}
