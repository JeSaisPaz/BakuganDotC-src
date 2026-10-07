// bdc 0x0889dad8 ActorStageObjCreateLights
#include "bdc.h"

/* On stages 0/13 (`GameStageIs0Or13`) adds light billboards to a stage object: finds the first of
   the 5 `g_stageObjLightSets` whose model name occurs in the object's model name (`strstr`), and
   for each of its lights creates a billboard on `g_billboardSpriteLayer` with the texture
   `g_stageObjLightTexNames``[texIndex]` (`GfxSpriteLayerCreateBillboardByName`), alpha 1, scale
   vector = the light's model-space position (w 0), position = that point transformed by the model's
   root matrix (`MathMtx4TransformPoint`), size 80, blend mode 2, and attaches it with
   `ActorStageObjAttachLight`. Returns without lights on other stages or when no set matches.
   Called by `ActorStageObjBaseCtor`. */

void ActorStageObjCreateLights(ActorStageObjBase *self)
{
    ScePspFVector4 local __attribute__((aligned(16)));
    ScePspFVector4 world __attribute__((aligned(16)));
    GfxSprite *sprite;
    u32 i;
    s32 j;

    if (GameStageIs0Or13() == 0) {
        return;
    }
    for (i = 0; i < 5; i++) {
        if (strstr(self->base.name, g_stageObjLightSets[i]->modelName) == NULL) {
            continue;
        }
        for (j = 0; j < g_stageObjLightSets[i]->count; j++) {
            sprite = GfxSpriteLayerCreateBillboardByName(
                g_billboardSpriteLayer,
                g_stageObjLightTexNames[(s32)g_stageObjLightSets[i]->lights[j].texIndex]);
            sprite->alpha = 1.0f;
            local.x = g_stageObjLightSets[i]->lights[j].pos[0];
            local.y = g_stageObjLightSets[i]->lights[j].pos[1];
            local.z = g_stageObjLightSets[i]->lights[j].pos[2];
            local.w = 0.0f;
            sprite->scaleX = local.x;
            sprite->scaleY = local.y;
            sprite->scaleZ = local.z;
            sprite->angle = local.w;
            MathMtx4TransformPoint((const ScePspFMatrix4 *)self->base.data->rootMatrix, &world, &local);
            sprite->posX = world.x;
            sprite->posY = world.y;
            sprite->posZ = world.z;
            sprite->posW = world.w;
            sprite->width = 80.0f;
            sprite->height = 80.0f;
            sprite->depth = 80.0f;
            sprite->maybe_sizeW = 0.0f;
            sprite->blendMode = 2;
            ActorStageObjAttachLight(self, sprite);
        }
        return;
    }
}
