// bdc 0x0887fec0 BtlAttackType45Update
#include "bdc.h"

/* Per-frame handler of attack type 0x45 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   runs `BtlAttackUpdateGuided` with turn 0.1, speed 0, hit kind 0x68, hit effect 0xdc and
   cancel effect 0xdc. When that returns non-zero (the attack ended) it stops every effect of
   `g_btlAttackEffectMgr` attached to the attack's `pos` (`GfxEffectStopAttached`, any id);
   otherwise, when the attack has an attached `effect`, it writes the effect's `dir` as the
   attack's velocity with xyz negated, then normalises xyz in place (each lane clamped to
   [-1, 1]; a zero vector stays zero) and sets `dir.w` to 0 (the bank's S713 left in the
   stored C710's w lane). */
void BtlAttackType45Update(BtlAttack *self)
{
    float *dir;
    float x, y, z, inv;

    if (BtlAttackUpdateGuided(0.100000001f, 0.0f, self, 0x68, 0xdc, 0xdc) != 0) {
        GfxEffectStopAttached(g_btlAttackEffectMgr, -1, self->pos);
        return;
    }
    if (self->effect == NULL) {
        return;
    }
    dir = ((GfxEffect *)self->effect)->dir;
    dir[0] = -self->vel[0];
    dir[1] = -self->vel[1];
    dir[2] = -self->vel[2];
    dir[3] = self->vel[3];

    dir = ((GfxEffect *)self->effect)->dir;
    x = dir[0];
    y = dir[1];
    z = dir[2];
    inv = x * x + y * y + z * z;
    if (inv == 0.0f) {
        inv = 0.0f;
    } else {
        inv = VfRsq(inv);
    }
    dir[0] = VfSat1(x * inv);
    dir[1] = VfSat1(y * inv);
    dir[2] = VfSat1(z * inv);
    dir[3] = 0.0f;
}
