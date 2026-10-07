// bdc 0x0884ccc4 BtlMainRebindListener
#include "bdc.h"

/* Rebinds the 3D sound listener to the battle's embedded follow camera `camera`
   (`BtlCameraBindListener`), then runs one stage-object system step
   (`ActorStageObjUpdateAll`). */
void BtlMainRebindListener(BtlMain *self)
{
    BtlCameraBindListener(&self->camera);
    ActorStageObjUpdateAll();
}
