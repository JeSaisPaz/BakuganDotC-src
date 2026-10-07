// bdc 0x088a9f24 ActorStageObjState06Update
#include "bdc.h"

/* State 6 handler of the shared stage-object state machine (state `+0x304`, `MemberFnPtr` table
   `0x08a842f8`, run by `ActorStageObjUpdate`): sink-and-fade collapse driven by `step`.
   Step 0 sets `restHeight` to 316 and goes to step 1; step 1 sinks the object (above 210.667 by
   14.0304 per frame, below by 7.0152 while jittering X/Z with `ActorStageObjGetShakeOffset`),
   marks the collider `attachDirty`, and advances once `restHeight < 0`; step 2 spawns the break
   model and remains, resets `fade` and jumps to step 10; step 10 fades `fade` by 0.2 per frame and
   advances at 0; step 11 sets `removeRequest`. Other steps do nothing. */

void ActorStageObjState06Update(ActorStageObjBase *self)
{
    float height;
    float baseX;
    float baseZ;
    int off;
    CollisionCollider *collider;

    switch (self->step) {
    case 0:
        height = 316.0f;
        self->restHeight = height;
        self->step++;
        goto sink;
    case 1:
        height = self->restHeight;
    sink:
        if (height < 0.0f) {
            self->step++;
        } else if (!(height <= 210.66667f)) {
            self->restHeight = height - 14.030399f;
            self->base.pos[1] = self->base.pos[1] - 14.030399f;
        } else {
            baseX = self->shakeX;
            off = ActorStageObjGetShakeOffset(self, self->shakePhase & 0x1f);
            baseZ = self->shakeZ;
            self->base.pos[0] = baseX + (float)(off / 2);
            off = ActorStageObjGetShakeOffset(self, (self->shakePhase + 8) & 0x1f);
            self->shakePhase++;
            self->base.pos[2] = baseZ + (float)(off / 2);
            self->base.pos[1] = self->base.pos[1] - 7.0151997f;
            self->restHeight = self->restHeight - 7.0151997f;
        }
        collider = (CollisionCollider *)self->collider;
        if (collider != NULL) {
            collider->attachDirty = 1;
        }
        break;
    case 2:
        ActorStageObjSpawnBreakModel(self);
        ActorStageObjSpawnRemains(self);
        self->fade = 0.0f;
        self->step = 10;
        /* fall through */
    case 10:
        self->fade = self->fade - 0.2f;
        if (self->fade <= 0.0f) {
            self->fade = 0.0f;
            self->step++;
        }
        break;
    case 11:
        self->removeRequest = 1;
        break;
    default:
        break;
    }
}
