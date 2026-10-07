// bdc 0x088971b8 BtlAiSetLevel
#include "bdc.h"

/* Sets the skill level of a CPU unit's AI object (the 0xa30-byte object created by
   `BtlAiCreate`): when it differs from `level` (`+0x1a4`) the value is stored and
   `BtlAiApplyLevel` re-derives the tuning parameters. */
void BtlAiSetLevel(BtlAi *self, u8 level)
{
    if (self->level != (s32)level) {
        self->level = level;
        BtlAiApplyLevel(self);
    }
}
