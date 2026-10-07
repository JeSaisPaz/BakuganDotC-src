// bdc 0x088b9d58 GameFieldCameraUpdate
#include "bdc.h"

/* Per-frame update of the field camera (embedded at `+0x20` of the field scene task,
   `GameFieldUpdate`; follow target `+0x2a0`, eye `+0x50`, look-at `+0x60`, mode `+0x2ac`): sets
   near/far (`+0x40 = 3.9`, `+0x44 = 30000.0`), runs the mode handler from the `MemberFnPtr` table
   `g_gameFieldCameraModeFns``[mode]` (0 `GameFieldCameraModeFollow`, 1 `GameFieldCameraModeTalk`, 2
   `GameFieldCameraModeTerminal`, 3 `GameFieldCameraFollowStep`, 4 `GameFieldCameraModeAim`, 5 `GameFieldCameraModeHold`, 6
   `GameFieldCameraModeBlend`, 7 `GameFieldCameraModeOrbit`, 8 `GameFieldCameraModeHeadView`), remembers the mode in
   `+0x2a4`, rebuilds the view (`GameFieldCameraApplyShake`, `GfxCameraUpdate(cam, -1)`) and, when a target
   is set, keeps `+0x2e0` halfway between the eye and the target's position. */

void GameFieldCameraUpdate(GameFieldCamera *cam)
{
    const MemberFnPtr *member;
    u8 *self;
    void *fn;

    cam->base.nearZ = 3.9f;
    cam->base.farZ = 30000.0f;

    member = &g_gameFieldCameraModeFns[cam->mode];
    self = (u8 *)cam + member->delta;
    if (member->index == 0) {
        fn = g_gameFieldCameraModeFns[cam->mode].pfn;
    } else {
        const VtblEntry *vtbl = *(const VtblEntry **)(self + (intptr_t)g_gameFieldCameraModeFns[cam->mode].pfn);
        const VtblEntry *entry = &vtbl[g_gameFieldCameraModeFns[cam->mode].index];

        fn = entry->fn;
        self += entry->delta;
    }
    ((void (*)(void *))fn)(self);

    cam->lastMode = cam->mode;
    GameFieldCameraApplyShake(cam);
    GfxCameraUpdate(&cam->base, 0xffffffff);

    if (cam->target != NULL) {
        const float *goal;
        float *pos = &cam->listenerPos.x;
        int i;

        for (i = 0; i < 4; i++)
            pos[i] = cam->base.eye[i];
        goal = ((Actor *)cam->target)->base.pos;
        for (i = 0; i < 4; i++)
            pos[i] = pos[i] + (goal[i] - pos[i]) * 0.5f;
    }
}
