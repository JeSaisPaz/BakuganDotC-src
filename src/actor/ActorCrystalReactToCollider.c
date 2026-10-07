// bdc 0x08857650 ActorCrystalReactToCollider
#include "bdc.h"

/* When the second collider `collider2` has registered a hit the crystal accepts
   (`ActorCrystalCanBeHitBy` on the collider's `hitParam164`), spawns the deflect spark 0x37
   (`GfxEffectSpawnDirected` on `g_btlUnitEffectMgr`) at the hit point, pointing along the
   normalised horizontal direction hit point → `anchorMatrix` row 3, plays sound 0x20006f and
   re-arms the collider (`flags` bit 0, `hitTimer` 1, `cooldown` 0). The heading
   `atan2f(hit.z - pos.z, hit.x - pos.x)` is computed either way, but its ring offset is scaled by
   0, so the spark position is the hit point itself (plus `cos/sin(heading) * 0`). */

void ActorCrystalReactToCollider(ActorCrystal *self)
{
    bool canHit;
    CollisionCollider *collider;
    float heading;
    float lenSq;
    float k;
    ScePspFVector4 hit;
    float pos[4];
    float dir[4];

    canHit = ActorCrystalCanBeHitBy(self, ((CollisionCollider *)self->collider2)->hitParam164);
    collider = (CollisionCollider *)self->collider2;
    hit = collider->hitPos;
    heading = atan2f(hit.z - self->base.base.pos[2], hit.x - self->base.base.pos[0]);
    if (canHit) {
        collider = (CollisionCollider *)self->collider2;
        /* dir = anchor row 3 - hit point, y forced to 0 */
        dir[0] = self->base.anchorMatrix[3][0] - collider->hitPos.x;
        dir[2] = self->base.anchorMatrix[3][2] - collider->hitPos.z;
        dir[1] = 0.0f;
        /* normalised (zero length: scale by S713 = 0), saturated to [-1, 1]; lane 3 is the masked
           lane of C710, i.e. S713 = 0 */
        lenSq = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
        k = VfRsq(lenSq);
        if (lenSq == 0.0f) {
            k = 0.0f;
        }
        dir[0] = VfSat1(dir[0] * k);
        dir[1] = VfSat1(dir[1] * k);
        dir[2] = VfSat1(dir[2] * k);
        dir[3] = 0.0f;
        /* heading * S703 fed to vrot [C,0,S,0], scaled by 0; lane 3 stays 0 */
        pos[0] = __builtin_cosf(heading) * 0.0f;
        pos[1] = 0.0f * 0.0f;
        pos[2] = __builtin_sinf(heading) * 0.0f;
        pos[3] = 0.0f;
        collider = (CollisionCollider *)self->collider2;
        pos[0] = pos[0] + collider->hitPos.x;
        pos[1] = pos[1] + collider->hitPos.y;
        pos[2] = pos[2] + collider->hitPos.z;
        GfxEffectSpawnDirected(g_btlUnitEffectMgr, 0x37, pos, dir);
        BtlBakuganPlaySound(&self->base, 0x20006f, 0, 0);
        collider = (CollisionCollider *)self->collider2;
        collider->flags |= 1;
        collider->hitTimer = 1;
        ((CollisionCollider *)self->collider2)->cooldown = 0;
    }
}
