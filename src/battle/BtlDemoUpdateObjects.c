// bdc 0x088ffa9c BtlDemoUpdateObjects
#include "bdc.h"

/* Per-frame object update shared by the battle intro demo task (task id 0x65, `BtlDemoCtor`) and
   the Bakugan-appearance demo task (task id 0x67, `BtlAppearDemoCtor`): runs the stage
   updates (`ActorStageObjRecordUpdateAll`, `BtlStageUpdateUvAnims`); holds the demo Bakugan's
   gravity for 10 frames and runs its motion update (vtable entry 7); keeps the demo actor's fall
   speed 0 and jump timer 1; updates the model chains (`GfxModelChainUpdateMotion` on
   `modelChain`, `models` and `mapModels`), `ActorStageObjUpdateAll`, the demo camera
   (`BtlDemoCamUpdate`, `GfxCameraUpdate` flags 2), the particle sets, the effect and sprite
   layers and `GfxMeshObjUpdateAll`. Finally it steps `flashBlend` by 0.05 (up, clamped to 1,
   while `focusValue` counts down; else down, clamped to 0), eases `flashColour` toward
   `focusPos` by s = (1 - cos(pi x blend)) / 2 and sets the flash alpha to s x `focusPos[3]`. */
void BtlDemoUpdateObjects(BtlDemo *demo)
{
    const VtblEntry *motion;
    float blend;
    float c;
    float s;
    int i;

    ActorStageObjRecordUpdateAll();
    BtlStageUpdateUvAnims();
    if (demo->bakugan != NULL) {
        demo->bakugan->gravityHold = 10;
        motion = &((const VtblEntry *)demo->bakugan->base.base.vtable)[7];
        ((void (*)(void *))motion->fn)((u8 *)demo->bakugan + motion->delta);
    }
    if (demo->actor != NULL) {
        demo->actor->fallSpeed = 0.0f;
        demo->actor->jumpTimer = 1;
    }
    GfxModelChainUpdateMotion(demo->modelChain);
    GfxModelChainUpdateMotion(demo->models->head);
    GfxModelChainUpdateMotion(demo->mapModels->head);
    ActorStageObjUpdateAll();
    BtlDemoCamUpdate(&demo->cam);
    GfxCameraUpdate(&demo->cam.base, 2);
    if (demo->particles0 != NULL) {
        GfxEffectMgrUpdate(demo->particles0);
    }
    if (demo->particles1 != NULL) {
        GfxEffectMgrUpdate(demo->particles1);
    }
    if (demo->effectLayer != NULL) {
        GfxSpriteLayerUpdateAll(demo->effectLayer);
    }
    GfxSpriteLayerUpdateAll(demo->spriteLayer);
    GfxMeshObjUpdateAll();

    if (demo->focusValue > 0) {
        blend = demo->flashBlend + 0.0500000007f;
        if (!(blend <= 1.0f)) {
            blend = 1.0f;
        }
        demo->flashBlend = blend;
        demo->focusValue--;
    } else {
        blend = demo->flashBlend - 0.0500000007f;
        if (blend < 0.0f) {
            blend = 0.0f;
        }
        demo->flashBlend = blend;
    }
    c = __builtin_cosf(demo->flashBlend * 3.14159274f);
    s = (1.0f - c) * 0.5f;
    /* flashColour += (focusPos - flashColour) * s, all four lanes */
    for (i = 0; i < 4; i++) {
        demo->flashColour[i] = demo->flashColour[i] + (demo->focusPos[i] - demo->flashColour[i]) * s;
    }
    c = __builtin_cosf(demo->flashBlend * 3.14159274f);
    demo->flashColour[3] = (1.0f - c) * 0.5f * demo->focusPos[3];
}
