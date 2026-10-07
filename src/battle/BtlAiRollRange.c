// bdc 0x088903a8 BtlAiRollRange
#include "bdc.h"

/* Picks a value from a rule's range for `BtlAi`: with `byLevel` set, the
   level-interpolated `min + (max - min) * ((level - 1) * 1/9)` (level `self->level`, converted as
   unsigned), otherwise `min + ``CoreRandFloat``(max - min)`. */
float BtlAiRollRange(BtlAi *self, BtlAiRange *range)
{
    float min = range->min;
    float span = range->max - min;

    if (range->byLevel != 0) {
        return min + span * ((float)(u32)(self->level - 1) * 0.111111112f);
    }
    return CoreRandFloat(span) + min;
}
