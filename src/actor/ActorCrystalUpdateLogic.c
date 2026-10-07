// bdc 0x08858770 ActorCrystalUpdateLogic
#include "bdc.h"

/* Crystal gameplay step; does nothing while the input is disabled and no scripted cast is armed
   (`scriptedCast`). Plays the hit shake (`shaking`: 12 frames of offsets from
   `ActorCrystalGetShakeOffset` added to `basePos` x/z in the model's root translation, then
   restores it), lets virtual slot 22 arm a hit on `collider0` (hit timer 0, flags 1|4), polls the
   colliders (`CollisionColliderTickHit`: `collider2` hits go to `ActorCrystalReactToCollider`,
   a `collider0` hit sets its timer to 4), clears state flags 0x14/0x620000 and the AI actions, then,
   unless the crystal is dead, runs the mode handler from `g_actorCrystalModeTable` (indexed by
   `mode`), `ActorCrystalUpdateSpellCast` and `ActorCrystalUpdateFade`, and calls the hit
   virtual (slot 24, `ActorCrystalOnHit`) when `collider0` was hit and there is no `auxObject`. */

void ActorCrystalUpdateLogic(ActorCrystal *self)
{
    const VtblEntry *entry;
    const MemberFnPtr *member;
    CollisionCollider *collider;
    u8 *obj;
    void *fn;
    float base;
    s32 frame;
    bool hit;

    if (self->base.input->disabled != 0 && self->scriptedCast == 0) {
        return;
    }
    hit = false;
    if (self->shaking != 0) {
        base = self->basePos[0];
        self->base.base.data->rootMatrix[12] =
            base + (float)ActorCrystalGetShakeOffset(self, self->shakeFrame & 0x1f);
        base = self->basePos[2];
        self->base.base.data->rootMatrix[14] =
            base + (float)ActorCrystalGetShakeOffset(self, (self->shakeFrame + 8) & 0x1f);
        frame = self->shakeFrame;
        self->shakeFrame = frame + 1;
        if (frame >= 12) {
            self->base.base.data->rootMatrix[12] = self->basePos[0];
            self->base.base.data->rootMatrix[14] = self->basePos[2];
            self->shaking = 0;
        }
    }
    if (self->base.collider0 != NULL) {
        entry = &((const VtblEntry *)self->base.base.base.vtable)[22];
        if (((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta) != 0) {
            collider = self->base.collider0;
            collider->hitTimer = 0;
            collider->flags = collider->flags | 1;
            collider = self->base.collider0;
            collider->flags = collider->flags | 4;
        }
    }
    collider = (CollisionCollider *)self->collider2;
    if (collider != NULL && CollisionColliderTickHit(&collider->node)) {
        ActorCrystalReactToCollider(self);
    }
    collider = self->base.collider0;
    if (collider != NULL && CollisionColliderTickHit(&collider->node)) {
        self->base.collider0->hitTimer = 4;
        hit = true;
    }
    self->base.stateFlags = self->base.stateFlags & 0xff9dffeb;
    self->base.input->aiActions = 0;
    if (self->base.combat.dead != 0) {
        return;
    }
    member = &g_actorCrystalModeTable[self->mode];
    obj = (u8 *)self + member->delta;
    fn = member->pfn;
    if (member->index != 0) {
        entry = &(*(const VtblEntry **)(obj + (uintptr_t)member->pfn))[member->index];
        fn = entry->fn;
        obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
    ActorCrystalUpdateSpellCast(self);
    ActorCrystalUpdateFade(self);
    if (self->auxObject == NULL && hit) {
        entry = &((const VtblEntry *)self->base.base.base.vtable)[24];
        ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
    }
}
