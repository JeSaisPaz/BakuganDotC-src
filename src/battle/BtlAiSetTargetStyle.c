// bdc 0x0888dedc BtlAiSetTargetStyle
#include "bdc.h"

/* Changes the targeting style of `BtlAi`: when `style` differs from `targetStyle`
   (`+0x1a8`), stores it and `arg` (`targetStyleArg`, `+0x1ac`) and re-applies the level, rule mode
   and target pickers (`BtlAiApplyLevel`, `BtlAiUpdateRuleMode`, `BtlAiSetTargetPickers`). */
void BtlAiSetTargetStyle(BtlAi *self, s32 style, s32 arg)
{
    if (self->targetStyle != style) {
        self->targetStyle = style;
        self->targetStyleArg = arg;
        BtlAiApplyLevel(self);
        BtlAiUpdateRuleMode(self);
        BtlAiSetTargetPickers(self);
    }
}
