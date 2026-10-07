// bdc 0x088f0fa8 GameEventOp80SetActorMotion
#include "bdc.h"

/* Handler of event opcode 0x80 (`GameEvent470ExecCommand`): writes the placed idle motion of
   actor `flag` into its placement record (`g_gameEventLocationBlock[i]`: motion index `+0x40` from `+0x28e`,
   loop bit, blend frames `arg` (default 6)) and replays it. */

void GameEventOp80SetActorMotion(GameEvent470 *self, u8 flag, s16 arg) {
    u32 actor = flag;
    u8 blend = (u8)arg;
    u8 **table = (u8 **)g_gameEventLocationBlock;

    if (actor == 100) {
        actor = self->talkPartner;
    }
    if (blend == 0) {
        blend = 6;
    }
    table[self->actorMap[actor]][0x12] = 0;
    table[self->actorMap[actor]][0x40] = (u8)self->motionIndex;
    table[self->actorMap[actor]][0x47] =
        (table[self->actorMap[actor]][0x47] & 0xfb) | (((self->flags470 & 4) == 0) << 2);
    table[self->actorMap[actor]][0x44] = blend;
}
