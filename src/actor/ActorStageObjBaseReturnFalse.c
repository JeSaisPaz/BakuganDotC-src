// bdc 0x088ac9c4 ActorStageObjBaseReturnFalse
#include "bdc.h"

/* Default virtual returning 0 (base stage-object vtable slot 19, inherited by every
   stage-object class except ActorStageObjProp's, which overrides it). */
int ActorStageObjBaseReturnFalse(ActorStageObjBase *self)
{
    (void)self;
    return 0;
}
