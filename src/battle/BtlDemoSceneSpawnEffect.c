// bdc 0x08905be0 BtlDemoSceneSpawnEffect
#include "bdc.h"

/* Spawns the effect for `.scb` event id `id` at `pos`: maps the id (`BtlDemoSceneMapEffectId`,
   which also reports whether the effect attaches to the unit and picks the pool); unmapped (-1)
   does nothing. Mapped ids 0x37 and 0xe0 instead set the demo's `flashOn` with `flashAlpha` 0.8.
   Otherwise adds the stage vector (`BtlGetStageAmbientColor`) to `pos.xyz` in place and spawns
   from the world pool `g_worldEffectMgr` or, with `stagePool`, `g_btlUnitEffectMgr`: at `pos`
   (`GfxEffectSpawn`) without `mtx`, else at the matrix (`GfxEffectSpawnAtMatrix`). An
   attaching effect, when the player has a unit and the effect exists, follows the translation row
   of the unit's `anchorMatrix`; mapped ids 0x70..0x75 instead set the unit's `subTimer` to 0xdead,
   call the unit's virtual slot 41 and follow its `effectAnchor`. */
void BtlDemoSceneSpawnEffect(BtlDemoScenePlayer *task, int id, float *pos, const float *mtx)
{
    float offset[4];
    u8 attach = 0;
    s32 effectId;
    GfxEffect *effect;
    BtlBakugan *unit;
    const VtblEntry *entry;

    effectId = BtlDemoSceneMapEffectId(task, id, &attach);
    if (effectId == -1) {
        return;
    }
    if (effectId == 0x37 || effectId == 0xe0) {
        ((BtlDemo *)task->demo)->flashOn = 1;
        ((BtlDemo *)task->demo)->flashAlpha = 0.800000012f;
        return;
    }
    BtlGetStageAmbientColor(offset);
    pos[0] = pos[0] + offset[0];
    pos[1] = pos[1] + offset[1];
    pos[2] = pos[2] + offset[2];
    if (mtx == NULL) {
        if (task->stagePool != 0) {
            effect = (GfxEffect *)GfxEffectSpawn(g_btlUnitEffectMgr, effectId, pos);
        } else {
            effect = (GfxEffect *)GfxEffectSpawn(g_worldEffectMgr, effectId, pos);
        }
    } else {
        if (task->stagePool != 0) {
            effect = GfxEffectSpawnAtMatrix(g_btlUnitEffectMgr, effectId, mtx);
        } else {
            effect = GfxEffectSpawnAtMatrix(g_worldEffectMgr, effectId, mtx);
        }
    }
    if (attach == 0 || task->unit == NULL || effect == NULL) {
        return;
    }
    unit = (BtlBakugan *)task->unit;
    effect->attachPos = unit->anchorMatrix[3];
    if (effectId < 0x70 || effectId >= 0x76) {
        return;
    }
    unit->subTimer = 0xdead;
    entry = &((const VtblEntry *)unit->base.base.vtable)[41];
    ((void (*)(void *))entry->fn)((u8 *)unit + entry->delta);
    effect->attachPos = ((BtlBakugan *)task->unit)->effectAnchor;
}
