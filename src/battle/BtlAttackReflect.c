// bdc 0x08877c58 BtlAttackReflect
#include "bdc.h"

/* Reflects an attack back at its owner, once: a second call only sets `cancelled` and clears
   `silentEnd` and `pendingHit`. The first call targets the old owner's id, makes `reflector` the
   owner, restarts the age at 1, clears cancelled/silentEnd/pendingHit, sets `reflected`, spawns the
   reflect effect 0x3d (`GfxEffectSpawnAttachedDir`) at the reflector's anchor (row 3 of
   `anchorMatrix`) pointing along normalise(pos - anchor) and tags it with the reflector. If the
   new target still exists (`BtlFindBakuganById`) it re-aims with `BtlAttackSteerToTarget`
   (turn rate 1, height 80), else it reverses the direction and copies it to the velocity; then
   sets the turn rate to 0.08 and copies the velocity into both effects' direction. The bounce
   direction is clamped per lane to [-1, 1], 0 for a zero-length offset, and its w is 0. */

void BtlAttackReflect(BtlAttack *self)
{
    float bounce[4] __attribute__((aligned(16)));
    GfxEffect *effect;
    BtlBakugan *reflector;

    if (self->reflected != 0) {
        self->cancelled = 1;
        self->silentEnd = 0;
        self->pendingHit = 0;
        return;
    }
    self->targetId = self->owner->base.base.id;
    self->owner = self->reflector;
    self->age = 1;
    self->cancelled = 0;
    self->silentEnd = 0;
    self->pendingHit = 0;
    self->reflected = 1;
    /* bounce = normalise(pos - anchor).xyz, each lane clamped to [-1, 1]; zero length gives 0.
       The w lane is the bank zero S713. */
    {
        const float *anchor = self->reflector->anchorMatrix[3];
        float dx = self->pos[0] - anchor[0];
        float dy = self->pos[1] - anchor[1];
        float dz = self->pos[2] - anchor[2];
        float lenSq = dx * dx + dy * dy + dz * dz;
        float k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
        bounce[0] = VfSat1(dx * k);
        bounce[1] = VfSat1(dy * k);
        bounce[2] = VfSat1(dz * k);
        bounce[3] = 0.0f;
    }
    effect = (GfxEffect *)GfxEffectSpawnAttachedDir(g_btlAttackEffectMgr, 0x3d,
                                                    self->reflector->anchorMatrix[3], bounce);
    reflector = self->reflector;
    effect->ownerBakugan = reflector;
    if (reflector != NULL) {
        effect->ownerId = reflector->base.base.id;
    }
    if (BtlFindBakuganById(self->targetId) != NULL) {
        self->turnRate = 1.0f;
        BtlAttackSteerToTarget(1.0f, 80.0f, self, 1, NULL);
    } else {
        self->dir[0] = -self->dir[0];
        self->dir[1] = -self->dir[1];
        self->dir[2] = -self->dir[2];
        self->vel[0] = self->dir[0];
        self->vel[1] = self->dir[1];
        self->vel[2] = self->dir[2];
        self->vel[3] = self->dir[3];
    }
    self->turnRate = 0.0799999982f;
    if (self->effect != NULL) {
        float *d = ((GfxEffect *)self->effect)->dir;
        d[0] = self->vel[0]; d[1] = self->vel[1]; d[2] = self->vel[2]; d[3] = self->vel[3];
    }
    if (self->effect2 != NULL) {
        float *d = ((GfxEffect *)self->effect2)->dir;
        d[0] = self->vel[0]; d[1] = self->vel[1]; d[2] = self->vel[2]; d[3] = self->vel[3];
    }
}
