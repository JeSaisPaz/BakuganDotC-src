// bdc 0x088f0fa8 GameEventOp80SetActorMotion
#include "bdc.h"

/* Handler of event opcode 0x80 (`GameEvent470ExecCommand`): writes the placed idle motion of
   actor `flag` into its placement record (`GameFieldPlacedChar` of `g_gameEventLocationBlock.placed`:
   motion index `frozenFlag` (+0x40) from `+0x28e`, loop bit 2 of `flags47`, blend frames `mode44` =
   `arg` (default 6), `routeFlag` cleared) and replays it. */

void GameEventOp80SetActorMotion(GameEvent470 *self, u8 flag, s16 arg) {
    u32 actor = flag;
    u8 blend = (u8)arg;
    GameFieldPlacedChar **table = g_gameEventLocationBlock.placed;

    if (actor == 100) {
        actor = self->talkPartner;
    }
    if (blend == 0) {
        blend = 6;
    }
    table[self->actorMap[actor]]->routeFlag = 0;
    table[self->actorMap[actor]]->frozenFlag = (u8)self->motionIndex;
    table[self->actorMap[actor]]->flags47 =
        (table[self->actorMap[actor]]->flags47 & 0xfb) | (((self->flags470 & 4) == 0) << 2);
    table[self->actorMap[actor]]->mode44 = blend;
}
