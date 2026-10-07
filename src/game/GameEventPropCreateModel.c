// bdc 0x088ea43c GameEventPropCreateModel
#include "bdc.h"

/* Creates the prop's model as an `ActorBallCreate``(0.01, 0x58, 0, 0)` object; for prop kind 0x14
   also starts its motion slot 2 (`ActorBallPlayMotion`). Returns 1 on success. */

s32 GameEventPropCreateModel(void *prop, s16 kind)
{
    CoreObject *model;

    if (kind == 0x14) {
        model = ActorBallCreate(0.01f, 0x58, 0, (float *)0);
        *(CoreObject **)prop = model;
        ActorBallPlayMotion(0.2f, model, 2, 1, false);
        model = *(CoreObject **)prop;
    } else {
        model = ActorBallCreate(0.01f, 0x58, 0, (float *)0);
        *(CoreObject **)prop = model;
    }
    if (model != (CoreObject *)0) {
        return 1;
    }
    return 0;
}
