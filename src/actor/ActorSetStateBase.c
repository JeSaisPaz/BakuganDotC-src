// bdc 0x088e059c ActorSetStateBase
#include "bdc.h"

/* Base-class state switch of the actors: stores `state` (`+0x140`); unless `keepVars`, resets the
   per-state fields (`stateTimer`, `aimTargetKind`, `cleared2d8`, `stateVec`, `aimPoint` and
   `stateVec300` = 0 (the last two from the bank zero C720), `footFlags`, `cleared310`), sets the
   motion speed to 1.0 through vtable slot `+0x30` (GfxModelSetMotionSpeed in the base vtable),
   clears `stateStep`, `waitTimer`, `stateDelay` and sets `stepCounter = 3`. Then per state: 0 idle
   (model 0x4d with a placement record: placed idle motion, selector 4 past the first route step,
   0 otherwise; any other actor: motion 0), 1 walk (motion 1), 9 nothing, 10 route walk (placed
   motion; selector 2 the first time / 3 afterwards for model 0x4d, 2 for 0x38/0x3b), 8 saves the
   placement (`ActorSavePlacement`) and then, like 2..7 and anything above 10, zeroes the velocity
   (stops the actor) and plays idle motion 0. */

void ActorSetStateBase(Actor *self, s32 state, u8 keepVars)
{
    const VtblEntry *entry;
    ActorNpcPlacement *place;
    u32 model;

    self->state = state;
    if (keepVars == 0) {
        self->stateTimer = 0;
        self->aimTargetKind = 0;
        self->cleared2d8 = 0;
        self->stateVec[0] = 0.0f;
        self->stateVec[1] = 0.0f;
        self->stateVec[2] = 0.0f;
        self->aimPoint[0] = 0.0f;
        self->aimPoint[1] = 0.0f;
        self->aimPoint[2] = 0.0f;
        self->aimPoint[3] = 0.0f;
        self->stateVec300[0] = 0.0f;
        self->stateVec300[1] = 0.0f;
        self->stateVec300[2] = 0.0f;
        self->stateVec300[3] = 0.0f;
        self->footFlags = 0;
        self->cleared310 = 0;
        entry = &((const VtblEntry *)self->base.base.vtable)[6];
        ((float (*)(void *, float))entry->fn)((u8 *)self + entry->delta, 1.0f);
        self->stateStep = 0;
        self->waitTimer = 0;
        self->stateDelay = 0;
        self->stepCounter = 3;
    }
    switch (self->state) {
    case 0:
        place = (ActorNpcPlacement *)self->placement;
        if (place != NULL && self->base.base.unk08 == 0x4d) {
            if (self->routeStep > 0) {
                place->frozenFlag = 4;
            } else {
                place->frozenFlag = 0;
            }
            ActorPlayPlacedMotion(self, 0);
        } else {
            ActorPlayMotion(0.2f, self, 0, 1, 0);
        }
        break;
    case 1:
        ActorPlayMotion(0.2f, self, 1, 1, 0);
        break;
    case 9:
        break;
    case 10:
        model = self->base.base.unk08;
        if (model == 0x4d) {
            place = (ActorNpcPlacement *)self->placement;
            if (self->routeWalkFirst != 0) {
                place->frozenFlag = 2;
                self->routeWalkFirst = 0;
            } else {
                place->frozenFlag = 3;
            }
        } else if (model == 0x3b || model == 0x38) {
            ((ActorNpcPlacement *)self->placement)->frozenFlag = 2;
        }
        ActorPlayPlacedMotion(self, 0);
        break;
    case 8:
        ActorSavePlacement(self);
        /* fall through */
    default:
        self->base.velocity[0] = 0.0f;
        self->base.velocity[1] = 0.0f;
        self->base.velocity[2] = 0.0f;
        self->base.velocity[3] = 0.0f;
        ActorPlayMotion(0.2f, self, 0, 1, 0);
        break;
    }
}
