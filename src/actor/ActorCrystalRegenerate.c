// bdc 0x08856788 ActorCrystalRegenerate
#include "bdc.h"

/* Crystal regeneration state machine on `regenStep` (+0xa34), only while `BtlIsScoreMode`(1):
   step 1 stops the hit shake, refills the HP (`BtlCombatGetMaxHp` converted as unsigned, then
   `BtlCombatSetHp`; snaps the HP gauge), moves the crystal to a free spawn point
   (`ActorCrystalPickFreeSpawnPoint`, `ActorStageObjRecordGetType4Pos`: position, `basePos`
   and the model matrix translation), sets `autoFire`, picks a new random colour
   (`ActorCrystalDefaultType`, `ActorCrystalRandomStyle`, `ActorCrystalSetStyle`) and goes
   to step 10. Step 10 turns lighting on, spawns effect 0x5b at the crystal (its `vec1e0[0]` =
   0.4 * scale x), plays sound `0x20025e`, clears the ambient alpha and waits 10 frames (step 11).
   Step 12 fades the model in: alpha = sinf(min(max(timer * 0.1396f, 0), pi/2)) on the
   model (`GfxModelScaleAmbientColor`), half of it on `"mat_spel"`
   (`GfxModelScaleAmbientColorByName`), stored in `fade`; once the alpha reaches 1 (or is NaN)
   step 13 resets mode 0 (`ActorCrystalSetMode`), revives the unit, clears the colliders' hit
   timer and flag bits 0x1/0x40/0x4 (collider0) and 0x40/0x4 (collider1), calls virtual entry 23
   (`+0xb8`, `ActorCrystalUpdateBoneAnchors` in the crystal vtable) and ends at step 0. Steps
   2..9 and any value outside 1..13 reset the step to 0.
   The VFPU bank constant S703 = 2/pi cancels the quarter turns of vsin.s: `sinf(angle)`. */

#define CRYSTAL_REGEN_HALF_PI 1.5707964f

void ActorCrystalRegenerate(ActorCrystal *self)
{
    float point[4];
    float *mtxPos;
    const VtblEntry *vtbl;
    GfxEffect *effect;
    float angle;
    float alpha;
    s32 t;

    if (BtlIsScoreMode(1) == 0) {
        return;
    }
    switch (self->regenStep) {
    case 1:
        self->shaking = 0;
        self->shakeFrame = 0;
        BtlCombatSetHp((float)(u32)BtlCombatGetMaxHp(&self->base.combat), &self->base.combat);
        if (self->base.hpGauge != NULL) {
            UiHpGaugeSnapToHp(self->base.hpGauge);
        }
        self->spawnPoint = ActorCrystalPickFreeSpawnPoint();
        ActorStageObjRecordGetType4Pos(point, (int)self->spawnPoint);
        /* point -> pos -> basePos, pos -> model matrix translation (all four lanes). */
        self->base.base.pos[0] = point[0];
        self->base.base.pos[1] = point[1];
        self->base.base.pos[2] = point[2];
        self->base.base.pos[3] = point[3];
        self->basePos[0] = self->base.base.pos[0];
        self->basePos[1] = self->base.base.pos[1];
        self->basePos[2] = self->base.base.pos[2];
        self->basePos[3] = self->base.base.pos[3];
        mtxPos = &self->base.base.data->rootMatrix[12];
        mtxPos[0] = self->base.base.pos[0];
        mtxPos[1] = self->base.base.pos[1];
        mtxPos[2] = self->base.base.pos[2];
        mtxPos[3] = self->base.base.pos[3];
        self->autoFire = 1;
        {
            u32 type = ActorCrystalDefaultType(self);
            ActorCrystalSetStyle(self, (s32)type, (s32)ActorCrystalRandomStyle(self));
        }
        self->regenStep = 10;
        /* fall through */
    case 10:
        self->base.base.lighting = 1;
        effect = (GfxEffect *)GfxEffectSpawn(g_btlUnitEffectMgr, 0x5b, self->base.base.pos);
        effect->vec1e0[0] = self->base.base.scale[0] * 0.4f;
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0x20025e, 0, 0);
        }
        self->base.base.ambient[3] = 0.0f;
        self->timer = 10;
        self->regenStep = self->regenStep + 1;
        /* fall through */
    case 11:
        t = self->timer;
        self->timer = t - 1;
        if (t > 0) {
            return;
        }
        self->timer = 0;
        self->regenStep = self->regenStep + 1;
        /* fall through */
    case 12:
        t = self->timer;
        angle = (float)t * 0.13962634f;
        self->timer = t + 1;
        if (angle < 0.0f) {
            angle = 0.0f;
        } else if (!(angle <= CRYSTAL_REGEN_HALF_PI)) {
            angle = CRYSTAL_REGEN_HALF_PI;
        }
        alpha = __builtin_sinf(angle);
        self->base.base.ambient[3] = alpha;
        GfxModelScaleAmbientColor(alpha, &self->base.base, NULL);
        GfxModelScaleAmbientColorByName(alpha * 0.5f, &self->base.base, "mat_spel");
        self->fade = alpha;
        if (alpha < 1.0f) {
            return;
        }
        self->base.base.lighting = 0;
        self->regenStep = self->regenStep + 1;
        /* fall through */
    case 13:
        self->base.base.lighting = 0;
        ActorCrystalSetMode(self, 0, false);
        self->fadeSkip = 0;
        self->base.combat.dead = 0;
        self->base.collider0->hitTimer = 0;
        self->base.collider0->flags &= ~0x1u;
        self->base.collider0->flags &= ~0x40u;
        self->base.collider0->flags &= ~0x4u;
        self->base.collider1->flags &= ~0x40u;
        self->base.collider1->flags &= ~0x4u;
        vtbl = &((const VtblEntry *)self->base.base.base.vtable)[23];
        ((void (*)(void *))vtbl->fn)((u8 *)self + vtbl->delta);
        self->regenStep = 999;
        self->regenStep = 0;
        break;
    default:
        /* steps 2..9 and anything outside 1..13 */
        self->regenStep = 0;
        break;
    }
}
