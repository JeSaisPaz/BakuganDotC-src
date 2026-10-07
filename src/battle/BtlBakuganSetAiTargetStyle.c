// bdc 0x0889acc4 BtlBakuganSetAiTargetStyle
#include "bdc.h"

/* Sets a CPU unit's AI targeting style: stores `style` in `BtlCpuUnit``.targetStyle` and, when
   the unit has an AI object (`ai`, `BtlAiCreate`), passes it on with
   `BtlAiSetTargetStyle``(ai, style, 0)`, which re-applies the target pickers only when the style
   changed. Counterpart of `BtlBakuganSetAiLevel` for the style. */
void BtlBakuganSetAiTargetStyle(BtlBakugan *self, s32 style)
{
    BtlCpuUnit *unit = (BtlCpuUnit *)self;
    BtlAi *ai = unit->ai;

    unit->targetStyle = style;
    if (ai != NULL) {
        BtlAiSetTargetStyle(ai, style, 0);
    }
}
