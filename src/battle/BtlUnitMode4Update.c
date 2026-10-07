// bdc 0x0885d9cc BtlUnitMode4Update
#include "bdc.h"

/* Per-frame update of the mode-4 battle unit (vtable `0x08af1e1c` slot `+0x38`): sets `started`,
   adds the distance moved since last frame (`|prevPos − pos|`, VFPU) to `travelled` and copies the
   current position (all four lanes) into `prevPos`. When `keepHpFull` is set
   and the HP gauge's `hitTimer × alpha` is at or below 0.4, sets the HP to the maximum
   (`BtlCombatGetMaxHp`, converted to float as unsigned; `BtlCombatSetHp`). Then runs
   `BtlBakuganUpdate`, the retreat step machine `BtlUnitMode4UpdateRetreat` and, for units that
   are not the local player (`BtlBakuganIsLocalPlayer`) whose `BtlBakuganRunBallEntry` returned
   non-zero, the AI (`BtlAiUpdate` on `ai`, when present). */

void BtlUnitMode4Update(BtlUnitMode4 *self)
{
    float dist;

    if (self->started == 0) {
        self->started = 1;
    }
    {
        float dx = self->prevPos[0] - self->base.base.pos[0];
        float dy = self->prevPos[1] - self->base.base.pos[1];
        float dz = self->prevPos[2] - self->base.base.pos[2];

        dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    }
    self->travelled = self->travelled + dist;
    self->prevPos[0] = self->base.base.pos[0];
    self->prevPos[1] = self->base.base.pos[1];
    self->prevPos[2] = self->base.base.pos[2];
    self->prevPos[3] = self->base.base.pos[3];
    if (self->keepHpFull != 0) {
        UiHpGauge *gauge = (UiHpGauge *)self->base.hpGauge;

        if (gauge->hitTimer * gauge->alpha <= 0.400000006f) {
            BtlCombatSetHp((float)(u32)BtlCombatGetMaxHp(&self->base.combat), &self->base.combat);
        }
    }
    BtlBakuganUpdate(&self->base);
    BtlUnitMode4UpdateRetreat(self);
    if (!BtlBakuganIsLocalPlayer(&self->base) && BtlBakuganRunBallEntry(&self->base) != 0 &&
        self->ai != NULL) {
        BtlAiUpdate(self->ai);
    }
}
