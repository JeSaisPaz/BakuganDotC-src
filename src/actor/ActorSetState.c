// bdc 0x088e2748 ActorSetState
#include "bdc.h"

/* State switch of the player actor (`ActorPlayerCtor`; `self` is an ActorPlayer): state 7 (throw)
   is refused (returns without any change) unless `ActorPlayerCanThrow` agrees. Leaving state 0xb
   releases the three hold effects (`holdEffects`, each from its manager with
   `UiSpriteLayerRelease`, slot cleared). Stores `state` (`+0x140`); unless `keepVars`, resets the
   per-state fields like `ActorSetStateBase` (`stateTimer`, `aimTargetKind`, `cleared2d8`,
   `stateVec`, `aimPoint`, `stateVec300` = 0 (bank zero C720), `footFlags`, `cleared310`, motion
   speed 1.0 through vtable slot `+0x30`, `stateStep`, `waitTimer`, `stateDelay`, `stepCounter = 3`)
   plus `aimState[0]` = 0 and `modelOffset` = 0. Then per state: 0 idle (clears `cleared510`,
   motion 0), 1 walk (motion 1), 7 throw (velocity x/z scaled by 0.8, y kept; motion 8, blend 0.5,
   no loop), 8 saves the placement (`ActorSavePlacement`) then, like every other state (2..6,
   9..12, above 12), zeroes the velocity (stops the actor) and plays idle motion 0. */

void ActorSetState(Actor *self, s32 state, s8 keepVars)
{
    ActorPlayer *player = (ActorPlayer *)self;
    const VtblEntry *entry;

    if (state == 7 && !ActorPlayerCanThrow(player)) {
        return;
    }
    if (self->state == 0xb) {
        if (player->holdEffects[0] != NULL) {
            UiSpriteLayerRelease(player->holdEffects[0]->mgr, player->holdEffects[0]);
            player->holdEffects[0] = NULL;
        }
        if (player->holdEffects[1] != NULL) {
            UiSpriteLayerRelease(player->holdEffects[1]->mgr, player->holdEffects[1]);
            player->holdEffects[1] = NULL;
        }
        if (player->holdEffects[2] != NULL) {
            UiSpriteLayerRelease(player->holdEffects[2]->mgr, player->holdEffects[2]);
            player->holdEffects[2] = NULL;
        }
    }
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
        player->aimState[0] = 0;
        player->modelOffset[0] = 0.0f;
        player->modelOffset[1] = 0.0f;
        player->modelOffset[2] = 0.0f;
        player->modelOffset[3] = 0.0f;
    }
    switch (self->state) {
    case 0:
        player->cleared510 = 0;
        ActorPlayMotion(0.2f, self, 0, 1, 0);
        break;
    case 1:
        ActorPlayMotion(0.2f, self, 1, 1, 0);
        break;
    case 7:
        self->base.velocity[0] = self->base.velocity[0] * 0.8f;
        self->base.velocity[2] = self->base.velocity[2] * 0.8f;
        ActorPlayMotion(0.5f, self, 8, 0, 0);
        break;
    case 8:
        ActorSavePlacement(self);
        /* fall through */
    default:
        /* states 11 and 12 have their own copies of this block in the binary */
        self->base.velocity[0] = 0.0f;
        self->base.velocity[1] = 0.0f;
        self->base.velocity[2] = 0.0f;
        self->base.velocity[3] = 0.0f;
        ActorPlayMotion(0.2f, self, 0, 1, 0);
        break;
    }
}
