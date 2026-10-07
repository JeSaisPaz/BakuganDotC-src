// bdc 0x088472f4 BtlCameraUpdate
#include "bdc.h"

/* Per-frame update of the battle camera controller (class vtable `0x08af186c` slot 2; the
   controller is embedded at `cameraTask + 0x20`, see `BtlGetCameraTask`): selects the per-unit
   camera parameters (`kindParams` = `g_btlCameraKindParams` row of the default target's kind),
   runs the handler of the camera mode through the pointer-to-member table `g_btlCameraModeFns`
   (0 `BtlCameraUpdateDefault`, 1 `BtlCameraUpdateLockOn`, 2 `BtlCameraUpdateFollow`), then
   copies the position of the followed object (mode 2) or of the default target (other modes, which
   also set the field of view to 50) into `listenerPos`, sets the near plane to 30 and runs
   `BtlCameraUpdateZoomPulse` and `GfxCameraUpdate` (flags 1). The parameter stays `void *` because
   the battle main task embeds the controller as a byte block. */
void BtlCameraUpdate(void *cameraObj)
{
    BtlCamera *camera = (BtlCamera *)cameraObj;
    const MemberFnPtr *member;
    u8 *self;
    void *fn;
    const float *src;

    if (camera->target == NULL) {
        /* debug hook; the binary passes "host0:../../disc/~CTBattleCamera.txt" in a0 */
        CoreDebugNop();
    }
    camera->kindParams =
        &g_btlCameraKindParams[((const BtlBakugan *)camera->target)->base.base.unk08];

    /* GCC 2.x pointer-to-member: index != 0 is a virtual slot, pfn then holds the vptr offset */
    member = &g_btlCameraModeFns[camera->mode];
    self = (u8 *)camera + member->delta;
    fn = member->pfn;
    if (member->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(self + (intptr_t)member->pfn);
        const VtblEntry *entry = &vtbl[member->index];

        fn = entry->fn;
        self += entry->delta;
    }
    ((void (*)(void *))fn)(self);

    if (camera->mode == 2) {
        src = ((const GfxModel *)camera->followTarget)->pos;
    } else {
        camera->base.fov = 50.0f;
        src = ((const GfxModel *)camera->target)->pos;
    }
    camera->listenerPos[0] = src[0];
    camera->listenerPos[1] = src[1];
    camera->listenerPos[2] = src[2];
    camera->listenerPos[3] = src[3];
    camera->base.nearZ = 30.0f;
    BtlCameraUpdateZoomPulse(camera);
    GfxCameraUpdate(&camera->base, 1);
}
