// bdc 0x08847430 BtlCameraSetTarget
#include "bdc.h"

/* Sets the camera's default target unit and its per-kind parameter row
   (`g_btlCameraKindParams` indexed by the unit kind, object word `+8`), copying the row's
   `param[3]` into the camera. */
void BtlCameraSetTarget(BtlCamera *camera, BtlBakugan *unit)
{
    const BtlCameraKindParams *row;

    camera->target = unit;
    row = &g_btlCameraKindParams[unit->base.base.unk08];
    camera->kindParams = row;
    camera->kindParam3 = row->param[3];
}
