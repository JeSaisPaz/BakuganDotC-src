// bdc 0x088308e4 BtlHudUpdateEnemyArrow
#include "bdc.h"

/* For an enemy at `pos` with arrow mode `mode` (0 none): projects the position with the camera
   (`GfxCameraProjectPoint`) and fetches the projection's C100 result (`MathVfpuStoreC100`);
   when its depth (element 2) is at most -3 (behind the camera) or the point is off screen
   (`GfxScreenPointIsOnScreen`), computes its bearing relative to the camera heading (`atan2f`
   on the camera eye x/z, minus `yaw`, wrapped into (-pi, pi]), picks the direction with
   `BtlHudAngleToArrowDir` and shows that arrow (`BtlHudShowEnemyArrow`). Returns the
   direction, or -1 when no arrow was shown. Part of the off-screen enemy arrows of the
   battle HUD (`BtlHudUpdate`). */

s32 BtlHudUpdateEnemyArrow(BtlHud *self, s32 mode, float *pos)
{
    /* GfxProjectToScreen (via GfxCameraProjectPoint) stores it with sv.q. */
    float screen[4] __attribute__((aligned(16)));
    float proj[4] __attribute__((aligned(16)));
    float target[4];
    ScePspFVector4 v;
    float angle;
    s32 dir = -1;

    v = GfxCameraProjectPoint(g_gfxActiveCamera, screen, pos);
    MathVfpuStoreC100(proj, v);
    if (mode == 0) {
        return dir;
    }
    if (!(proj[2] <= -3.0f) && GfxScreenPointIsOnScreen(screen) != 0) {
        return dir;
    }
    /* target = *pos (one 16-byte quad copy) */
    target[0] = pos[0];
    target[1] = pos[1];
    target[2] = pos[2];
    target[3] = pos[3];
    angle = atan2f(target[2] - g_gfxActiveCamera->eye[2], target[0] - g_gfxActiveCamera->eye[0]);
    angle = -g_gfxActiveCamera->yaw - angle;
    if (!(angle <= 3.14159274f)) {
        angle = angle - 6.28318548f;
    } else if (angle <= -3.14159274f) {
        angle = angle + 6.28318548f;
    }
    dir = BtlHudAngleToArrowDir(angle, self);
    BtlHudShowEnemyArrow(self, dir, mode);
    return dir;
}
