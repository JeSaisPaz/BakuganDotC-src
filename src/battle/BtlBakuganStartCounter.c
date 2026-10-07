// bdc 0x088712d8 BtlBakuganStartCounter
#include "bdc.h"

/* Counter-attack reaction (called by `BtlBakuganOnHit`): hits the recorded attacker back
   (`BtlBakuganInflictCounterHit`, strong when `perfect`), spawns effect 0x3a on the guard anchor
   pointing against the current velocity, enters state 0x13 (`BtlBakuganSetState`), sets the
   velocity to the vector towards the attacker and turns to face it (`BtlBakuganSetHeading` with
   atan2f(z, x)). The effect point is the guard anchor plus 100 units towards the attacker. With
   `perfect` it sets the dash heading to 0.3, spawns effect 0x3d there, clears the body collider's
   hit cooldown, plays sound 0x20008c and sets the velocity to 5 units away from the attacker;
   otherwise it clears the cooldown, plays sound 0x20008b, applies the energy of state 10
   (`BtlBakuganApplyStateEnergy`) and sets the velocity to 80 units away. Then it spawns effect 0x3b
   at the effect point pushed 30 units towards the camera (`GfxCameraPushPointAway`) and the
   attached effect 0x3c on the unit position, sets state flag 0x40, counts a counter (stat 0xb) in the
   battle stats and clears the counter window (`BtlBakuganResetCounterWindow`). Every normalisation
   uses scale 0 for a zero-length vector (bank S713) and stores w = 0 (C710's lane 3 is S713). */

void BtlBakuganStartCounter(BtlBakugan *self, char perfect)
{
    float point[4];
    float lenSq;
    float k;
    GfxEffect *effect;
    GfxEffectMgr *mgr;
    BtlBakugan *attacker;
    float *pushed;

    BtlBakuganInflictCounterHit(self, self->attacker, perfect);
    /* point = -normalize(velocity.xyz), w = 0 */
    point[0] = self->base.velocity[0];
    point[1] = self->base.velocity[1];
    point[2] = self->base.velocity[2];
    lenSq = point[0] * point[0] + point[1] * point[1] + point[2] * point[2];
    k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
    k = k * -1.0f;
    point[0] = point[0] * k;
    point[1] = point[1] * k;
    point[2] = point[2] * k;
    point[3] = 0.0f;
    effect = (GfxEffect *)GfxEffectSpawnAttachedDir(g_worldEffectMgr, 0x3a, self->guardAnchor, point);
    effect->ownerBakugan = self;
    if (self != NULL) {
        effect->ownerId = self->base.base.id;
    }
    BtlBakuganSetState(self, 0x13, 0);
    attacker = self->attacker;
    /* point = velocity = attacker.pos - pos (xyz), w = attacker's w */
    point[0] = attacker->base.pos[0] - self->base.pos[0];
    point[1] = attacker->base.pos[1] - self->base.pos[1];
    point[2] = attacker->base.pos[2] - self->base.pos[2];
    point[3] = attacker->base.pos[3];
    self->base.velocity[0] = point[0];
    self->base.velocity[1] = point[1];
    self->base.velocity[2] = point[2];
    self->base.velocity[3] = point[3];
    BtlBakuganSetHeading(self, atan2f(point[2], point[0]));
    /* point.xyz = normalize(point.xyz) * 100 + guardAnchor.xyz, w = 0 */
    lenSq = point[0] * point[0] + point[1] * point[1] + point[2] * point[2];
    k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
    k = k * 100.0f;
    point[0] = point[0] * k;
    point[1] = point[1] * k;
    point[2] = point[2] * k;
    point[3] = 0.0f;
    point[0] = point[0] + self->guardAnchor[0];
    point[1] = point[1] + self->guardAnchor[1];
    point[2] = point[2] + self->guardAnchor[2];
    if (perfect != 0) {
        self->dashHeading = 0.300000012f;
        effect = (GfxEffect *)GfxEffectSpawnAttachedDir(g_worldEffectMgr, 0x3d, self->guardAnchor, point);
        effect->ownerBakugan = self;
        if (self != NULL) {
            effect->ownerId = self->base.base.id;
        }
        self->collider0->cooldown = 0;
        BtlBakuganPlaySound(self, 0x20008c, 0, 0);
        /* velocity.xyz = -5 * normalize(velocity.xyz), w = 0 */
        k = -5.0f;
    } else {
        self->collider0->cooldown = 0;
        BtlBakuganPlaySound(self, 0x20008b, 0, 0);
        BtlBakuganApplyStateEnergy(self, 10, 0);
        /* velocity.xyz = -80 * normalize(velocity.xyz), w = 0 */
        k = -80.0f;
    }
    lenSq = self->base.velocity[0] * self->base.velocity[0] + self->base.velocity[1] * self->base.velocity[1] +
            self->base.velocity[2] * self->base.velocity[2];
    k = ((lenSq == 0.0f) ? 0.0f : VfRsq(lenSq)) * k;
    self->base.velocity[0] = self->base.velocity[0] * k;
    self->base.velocity[1] = self->base.velocity[1] * k;
    self->base.velocity[2] = self->base.velocity[2] * k;
    self->base.velocity[3] = 0.0f;
    mgr = g_worldEffectMgr;
    pushed = GfxCameraPushPointAway(-30.0f, point);
    GfxEffectSpawn(mgr, 0x3b, pushed);
    GfxEffectSpawnAttached(g_worldEffectMgr, 0x3c, self->base.pos);
    self->stateFlags = self->stateFlags | 0x40;
    if (self->stats != NULL) {
        BtlStatsAddCounter(self->stats, 0xb, 1);
    }
    BtlBakuganResetCounterWindow(self);
}
