// bdc 0x088b86c0 ActorBallUpdate
#include "bdc.h"

/* Update (vtable slot 7) of the `ActorBall` (`ActorBallCtor`): calls the state handler for
   `+0x150` from the 2-entry member-pointer table `0x08a90898` (0 idle `0x088b8c8c`, 1
   `ActorBallStateThrown`), advances the model animation (`GfxModelUpdateMotion`,
   `GfxModelApplyMotion`), rebuilds the matrix (`ActorBallUpdateMatrix`) and probes the ground
   (`ActorBallUpdateGround`). */

void ActorBallUpdate(CoreObject *ball)
{
    ActorBall *self = (ActorBall *)ball;
    s32 state = self->state;

    if (state >= 0 && state < 2) {
        const MemberFnPtr *member = &g_actorBallStateFns[state];
        u8 *obj = (u8 *)ball + member->delta;
        void *fn = member->pfn;

        if (member->index != 0) {
            const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)member->pfn);
            const VtblEntry *entry = &vtbl[member->index];

            fn = entry->fn;
            obj += entry->delta;
        }
        ((void (*)(void *))fn)(obj);
    }
    GfxModelUpdateMotion((GfxModel *)ball);
    GfxModelApplyMotion((GfxModel *)ball);
    ActorBallUpdateMatrix(self);
    ActorBallUpdateGround(ball);
}
