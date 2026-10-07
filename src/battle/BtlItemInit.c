// bdc 0x088b6194 BtlItemInit
#include "bdc.h"

/* Initialises a battle pickup item of `type`: clears its state, counters and flags (lifetime 500),
   spawns its glow effect `0x4d + type` on `g_btlItemEffectMgr` attached to its position, and for
   types 0..3 builds its model from the low heap (0x140-byte `GfxModelCtor`): 0
   `btl_01_gatecard.gmo`, 1 `btl_03_power_up.gmo`, 2 `btl_04_defense_up.gmo`, 3 `btl_02_heal.gmo`
   (other types get none). A model gets the stage light colours (`BtlStageApplyLightColors`),
   outside stages 0/13 an ambient scaled by 0.7 with alpha 1, lighting on, a root matrix scaled
   by 10 positioned at the item, material 0 shade-mode bits 5..7 = 2 and blend bits 0..1 = 2, and
   is appended to `g_btlItemObjList`. */

void BtlItemInit(BtlItem *self, s32 type)
{
    const char *name;
    GfxModel *model;
    GfxMaterialState *mat;
    float *root;
    void *mem;
    bool fromLow;
    float *ambient;
    s32 i;

    self->state = 0;
    self->reserved58 = 0.0f;
    self->age = 0;
    self->lifetime = 500;
    self->pickedUp = 0;
    self->effect = NULL;
    self->type = type;
    self->popFrame = 0;
    self->model = NULL;
    self->blinking = 0;
    self->appearAnim = 0;
    self->effect = GfxEffectSpawnAttached(g_btlItemEffectMgr, type + 0x4d, self->pos);
    switch ((u32)self->type) {
    case 0:
        name = "btl_01_gatecard.gmo";
        break;
    case 1:
        name = "btl_03_power_up.gmo";
        break;
    case 2:
        name = "btl_04_defense_up.gmo";
        break;
    case 3:
        name = "btl_02_heal.gmo";
        break;
    default:
        name = NULL;
        break;
    }
    if (name != NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = MemAlloc(0x140, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        model = NULL;
        if (mem != NULL) {
            GfxModelCtor(mem, name, 0);
            model = mem;
        }
        self->model = model;
    }
    if (self->model == NULL) {
        return;
    }
    BtlStageApplyLightColors(self->model);
    if (GameStageIs0Or13() == 0) {
        ambient = self->model->ambient;
        for (i = 0; i < 4; i++) {
            ambient[i] = ambient[i] * 0.7f;
        }
        self->model->ambient[3] = 1.0f;
    }
    self->model->lighting = 1;
    root = self->model->data->rootMatrix;
    root[10] = 10.0f;
    root[5] = 10.0f;
    root[0] = 10.0f;
    root = self->model->data->rootMatrix;
    for (i = 0; i < 4; i++) {
        root[12 + i] = self->pos[i];
    }
    mat = GfxModelGetMaterialState(self->model, 0);
    mat->shadeFlags = (u8)((mat->shadeFlags & ~0xe0) | 0x40);
    mat = GfxModelGetMaterialState(self->model, 0);
    mat->renderFlags = (u8)((mat->renderFlags & ~3) | 2);
    CoreObjectListAppend(&self->model->base, g_btlItemObjList);
}
