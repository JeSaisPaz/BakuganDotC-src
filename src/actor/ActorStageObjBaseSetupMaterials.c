// bdc 0x088ac814 ActorStageObjBaseSetupMaterials
#include "bdc.h"

/* Empty default of the material-setup virtual (base vtable `0x08af2904` slot 10; overridden e.g. by
   `ActorStageObjMineSetupMaterials`): does nothing. */
void ActorStageObjBaseSetupMaterials(ActorStageObjBase *self)
{
    (void)self;
}
