// bdc 0x088ed2e8 GameEventOp2APropCreate
#include "bdc.h"

/* Handler of event opcode 0x2a (`GameEventExecCommand`): creates a prop of kind `arg`
   (`GameEventPropCreate`) in event prop slot `flag` (100-byte records at `ev+0x30`, prop object
   at `+0x30`, see `GameEventPropCreate`) and copies its scale (or 1 when creation failed) into
   the record's scale keys `+0x34..+0x40`. */

void GameEventOp2APropCreate(GameEvent *self, u8 flag, s16 arg)
{
    s32 scale;

    self->props[flag].prop = GameEventPropCreate(arg);
    if (self->props[flag].prop == (void *)0) {
        scale = 1;
        self->props[flag].baseScale = scale;
        self->props[flag].startScale = scale;
        self->props[flag].endScale = scale;
    } else {
        scale = ((GameEventProp *)self->props[flag].prop)->scaleX;
        self->props[flag].baseScale = scale;
        self->props[flag].startScale = scale;
        self->props[flag].endScale = scale;
    }
    self->props[flag].scale = scale;
}
