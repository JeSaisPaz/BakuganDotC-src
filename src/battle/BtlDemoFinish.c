// bdc 0x088fef84 BtlDemoFinish
#include "bdc.h"

/* Ends a battle demo and removes its task (shared by the battle intro demo task (task id 0x65,
   `BtlDemoCtor`, update `BtlDemoUpdate` / draw `BtlDemoDraw` on the state `+0x65c`) and the
   Bakugan-appearance demo task (task id 0x67, `BtlAppearDemoCreate`, `BtlAppearDemoCtor`,
   update `BtlAppearDemoUpdate`; derived from the battle intro demo)): deferred-deletes the demo
   actor, frees the appear motions in game mode 1 (`ScriptVarsModeIs1`,
   `ActorEditManFreeAppearMotions`), kills the scene player tasks 0x66
   (`CoreTaskRemoveAllById`), waits for the GE (`GfxWaitGeIdle`), destroys the two particle
   managers (virtual destructor, flags 3), the model chain (`GfxModelChainDeleteAll`), the ball
   models (`ActorBallFreeMotions` on each, then virtual destructor) and deferred-deletes the
   stage model; resets the first arena model's ambient alpha to 1.0 and the effect managers
   `g_worldEffectMgr` / `g_btlUnitEffectMgr`, and restores the demo Bakugan's ambient alpha and
   its uniform scale (`stats->modelScale`, w 0). Then, when the battle task (id 100) exists, hands
   the battle back: `BtlMainOnDemoEnd`; with a player Bakugan and a demo Bakugan, re-places every
   unit of the Bakugan list whose virtual slot 14 returns 0 at the next saved placement
   (`BtlDemoPlaceBakugan`) and clears its body collider's hit (timer 0, flag bit 0), and resets
   the player's ambient alpha to 1.0; restores the active camera `g_gfxActiveCamera` (stopping its
   shake, `GfxCameraStopShake`) and clears the battle task's flags 3. Clears `flashAlpha`, frees
   the demo motions (`BtlDemoFreeMotions`) and finally `CoreTaskRemove``(demo, true)`. */
void BtlDemoFinish(CoreTask *demo)
{
    float scale[4] __attribute__((aligned(16)));
    float place[4] __attribute__((aligned(16)));
    BtlDemo *self;
    BtlMain *main;
    BtlBakugan *player;
    BtlBakugan *bakugan;
    BtlBakugan *unit;
    CoreObjectList *list;
    CollisionCollider *collider;
    const VtblEntry *entry;
    float (*placement)[4];
    float s;

    self = (BtlDemo *)demo;
    if (self->actor != NULL) {
        CoreObjectDeferDelete((CoreObject *)self->actor, 0);
        self->actor = NULL;
    }
    if (ScriptVarsModeIs1()) {
        ActorEditManFreeAppearMotions();
    }
    CoreTaskRemoveAllById(0x66);
    GfxWaitGeIdle();
    if (self->particles0 != NULL) {
        if (self->particles0 != NULL) {
            entry = &self->particles0->base.vtbl[1];
            ((void (*)(void *, s32))entry->fn)((u8 *)self->particles0 + entry->delta, 3);
        }
        self->particles0 = NULL;
    }
    if (self->particles1 != NULL) {
        if (self->particles1 != NULL) {
            entry = &self->particles1->base.vtbl[1];
            ((void (*)(void *, s32))entry->fn)((u8 *)self->particles1 + entry->delta, 3);
        }
        self->particles1 = NULL;
    }
    GfxModelChainDeleteAll(self->modelChain);
    if (self->balls[0] != NULL) {
        ActorBallFreeMotions(self->balls[0]);
    }
    if (self->balls[1] != NULL) {
        ActorBallFreeMotions(self->balls[1]);
    }
    if (self->balls[0] != NULL) {
        if (self->balls[0] != NULL) {
            entry = &((const VtblEntry *)self->balls[0]->vtable)[1];
            ((void (*)(void *, s32))entry->fn)((u8 *)self->balls[0] + entry->delta, 3);
        }
        self->balls[0] = NULL;
    }
    if (self->balls[1] != NULL) {
        if (self->balls[1] != NULL) {
            entry = &((const VtblEntry *)self->balls[1]->vtable)[1];
            ((void (*)(void *, s32))entry->fn)((u8 *)self->balls[1] + entry->delta, 3);
        }
        self->balls[1] = NULL;
    }
    if (self->stageModel != NULL) {
        CoreObjectDeferDelete(&self->stageModel->base, 0);
    }
    g_btlArenaModels[0]->ambient[3] = 1.0f;
    g_worldEffectMgr = NULL;
    g_btlUnitEffectMgr = NULL;
    if (self->bakugan != NULL) {
        bakugan = self->bakugan;
        bakugan->base.ambient[3] = 1.0f;
        s = bakugan->combat.stats->modelScale;
        scale[0] = s;
        scale[1] = s;
        scale[2] = s;
        scale[3] = 0.0f;
        bakugan->base.scale[0] = scale[0];
        bakugan->base.scale[1] = scale[1];
        bakugan->base.scale[2] = scale[2];
        bakugan->base.scale[3] = scale[3];
    }
    main = (BtlMain *)CoreTaskFind(100);
    if (main != NULL) {
        BtlMainOnDemoEnd(main);
        player = (BtlBakugan *)BtlGetPlayerBakugan();
        if (player != NULL && self->bakugan != NULL) {
            list = (CoreObjectList *)BtlGetBakuganList();
            if (list != NULL) {
                placement = self->placements;
                for (unit = (BtlBakugan *)list->head; unit != NULL;
                     unit = (BtlBakugan *)unit->base.base.next) {
                    entry = &((const VtblEntry *)unit->base.base.vtable)[14];
                    if (((s32 (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0) {
                        continue;
                    }
                    place[0] = (*placement)[0];
                    place[1] = (*placement)[1];
                    place[2] = (*placement)[2];
                    place[3] = (*placement)[3];
                    BtlDemoPlaceBakugan(self, unit, place);
                    if (unit->collider0 != NULL) {
                        collider = (CollisionCollider *)unit->collider0;
                        collider->hitTimer = 0;
                        collider->flags &= ~1u;
                    }
                    placement++;
                }
            }
            player->base.ambient[3] = 1.0f;
        }
        if (self->battleCamera != NULL) {
            g_gfxActiveCamera = self->battleCamera;
            GfxCameraStopShake(self->battleCamera);
        }
        CoreTaskClearFlags(&main->base, 3);
    }
    self->flashAlpha = 0.0f;
    BtlDemoFreeMotions(self);
    CoreTaskRemove(demo, true);
}
