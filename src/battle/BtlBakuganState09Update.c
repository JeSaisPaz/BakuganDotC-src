// bdc 0x0886b500 BtlBakuganState09Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 9 (`state`), vtable slot `+0x118` called through
   `BtlBakuganRunState`. Unless `BtlBakuganTryCancelIntoArt` switched into a special art, it
   sets bit 0x1000000 of `stateFlags`; for the local player (`BtlBakuganIsLocalPlayer`) in attack
   phase < 3 it spawns effect 0x1d twice at its position on `g_worldEffectMgr` directed along the
   normalised velocity (`GfxEffectSpawnDirected`). Then it runs the state-7 handler through
   vtable entry 33 (`+0x108`). In attack phase < 3 it also sets bit 0 of `stateFlags` and sets
   `orient` to (-0.9 * normalised velocity.y, 1, 0, 0), rotated in the xz plane by the angle
   rot.y (x' = o.(cos, 0, -sin), z' = o.(sin, 0, cos)) and normalised. If the state is no
   longer 9 afterwards it sets the input's `chargeCooldown` to 10.
   Every normalisation scales x, y, z by 1/sqrt of the squared length (0 when that is zero) with
   each component clamped to [-1, 1], and stores w = 0 (the bank's S713). */

void BtlBakuganState09Update(BtlBakugan *self)
{
    float dir[4];
    float norm[4];
    float lenSq;
    float k;
    float angle;
    float c;
    float s;
    float ox;
    float oz;
    const VtblEntry *entry;

    if (BtlBakuganTryCancelIntoArt(self) != 0) {
        return;
    }
    self->stateFlags |= 0x1000000;
    if (BtlBakuganIsLocalPlayer(self) && (s32)self->attackPhase < 3) {
        /* dir = normalised velocity */
        lenSq = self->base.velocity[0] * self->base.velocity[0]
              + self->base.velocity[1] * self->base.velocity[1]
              + self->base.velocity[2] * self->base.velocity[2];
        k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
        dir[0] = VfSat1(self->base.velocity[0] * k);
        dir[1] = VfSat1(self->base.velocity[1] * k);
        dir[2] = VfSat1(self->base.velocity[2] * k);
        dir[3] = 0.0f;
        GfxEffectSpawnDirected(g_worldEffectMgr, 0x1d, self->base.pos, dir);
        GfxEffectSpawnDirected(g_worldEffectMgr, 0x1d, self->base.pos, dir);
    }
    entry = &((const VtblEntry *)self->base.base.vtable)[33];
    ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
    if ((s32)self->attackPhase < 3) {
        self->stateFlags |= 1;
        lenSq = self->base.velocity[0] * self->base.velocity[0]
              + self->base.velocity[1] * self->base.velocity[1]
              + self->base.velocity[2] * self->base.velocity[2];
        k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
        norm[0] = VfSat1(self->base.velocity[0] * k);
        norm[1] = VfSat1(self->base.velocity[1] * k);
        norm[2] = VfSat1(self->base.velocity[2] * k);
        norm[3] = 0.0f;
        self->orient[0] = norm[1] * -0.899999976f;
        self->orient[1] = 1.0f;
        self->orient[2] = 0.0f;
        self->orient[3] = 0.0f;
        /* rotate xz by rot.y (vrot of rot.y * 2/pi in quarter turns) */
        angle = self->base.rot[1];
        c = __builtin_cosf(angle);
        s = __builtin_sinf(angle);
        ox = self->orient[0] * c + self->orient[1] * 0.0f + self->orient[2] * -s;
        oz = self->orient[0] * s + self->orient[1] * 0.0f + self->orient[2] * c;
        self->orient[0] = ox;
        self->orient[2] = oz;
        /* normalise orient */
        lenSq = self->orient[0] * self->orient[0]
              + self->orient[1] * self->orient[1]
              + self->orient[2] * self->orient[2];
        k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
        self->orient[0] = VfSat1(self->orient[0] * k);
        self->orient[1] = VfSat1(self->orient[1] * k);
        self->orient[2] = VfSat1(self->orient[2] * k);
        self->orient[3] = 0.0f;
    }
    if (self->state != 9) {
        self->input->chargeCooldown = 10;
    }
}
