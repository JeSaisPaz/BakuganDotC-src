// bdc 0x08848b10 BtlFinishTaskCtor
#include "bdc.h"

/* Constructor of the end-of-battle cinematic task (task id 0x14a, `BtlFinishTask`,
   `CoreTaskInit` then vtable `g_btlFinishTaskVtbl`). Builds the cinematic camera
   (`GfxCameraCtor`, `GfxCameraInit`; near 20, far 35000, fov 50) and stores `unit`, `arg` and
   `mode`. The camera direction is the unit's heading + 90 degrees rotated about Y with y = 0.4,
   normalised; distance 2000. With mode 0 the focus is the unit's "Bip01" node
   (`GfxModelGetNodeWorldPos`): it attaches effect 0x82 to the camera target
   (`GfxEffectSpawnAttached`, effect textureSlot = vtable entry 20 of the unit's last melee attacker,
   0 without one), plays the unit's motion 0xee (`BtlBakuganPlayMotion`, forced), and when that
   attacker exists and its vtable entry 11 returns 0 holds it at its motion frame - 2 clamped to
   [0, 1]. With mode 1 the focus is the unit position + 100 in y; other modes leave
   the focus uninitialised as the binary does. Eye = focus + dir * distance, target = focus;
   updates the camera (`GfxCameraUpdate` all flags), clears flag2f0 and makes the camera the
   active one, saving the previous `g_gfxActiveCamera`. Returns `self`. */

BtlFinishTask *BtlFinishTaskCtor(BtlFinishTask *self, void *unit, s32 arg, s32 mode)
{
    float focus[4];
    ScePspFVector4 node;
    BtlBakugan *bakugan;
    BtlBakugan *attacker;
    const VtblEntry *entry;
    GfxEffect *effect;
    s32 attackerValue;
    float angle;
    float frame;
    float x;
    float y;
    float z;
    float len2;
    float inv;
    float dist;

    CoreTaskInit(&self->base);
    self->base.vtable = g_btlFinishTaskVtbl;
    GfxCameraCtor(&self->camera.base);
    self->unit = unit;
    self->arg = arg;
    self->farCamera = mode;
    GfxCameraInit(&self->camera);
    self->camera.nearZ = 20.0f;
    self->camera.farZ = 35000.0f;
    self->camera.fov = 50.0f;
    angle = ((BtlBakugan *)self->unit)->base.rot[1] + 1.57079637f;
    /* vrot [C,0,S,0] of angle * S703 (2/pi) */
    self->camDir[0] = __builtin_cosf(angle);
    self->camDir[1] = 0.0f;
    self->camDir[2] = __builtin_sinf(angle);
    self->camDir[3] = 0.0f;
    self->camDir[1] = 0.400000006f;
    /* normalise xyz (1/sqrt, 0 for a zero length), each lane saturated to [-1, 1];
       w is the masked lane of C710, the bank's S713 = 0 */
    x = self->camDir[0];
    y = self->camDir[1];
    z = self->camDir[2];
    len2 = x * x + y * y + z * z;
    inv = VfRsq(len2);
    if (len2 == 0.0f) {
        inv = 0.0f;
    }
    self->camDir[0] = VfSat1(x * inv);
    self->camDir[1] = VfSat1(y * inv);
    self->camDir[2] = VfSat1(z * inv);
    self->camDir[3] = 0.0f;
    self->camDistance = 2000.0f;
    self->camBlend = 0.0f;
    self->flashT = 0.0f;
    self->step = 0;
    self->timer = 0;
    if (self->farCamera == 0) {
        bakugan = (BtlBakugan *)self->unit;
        GfxModelGetNodeWorldPos(&bakugan->base, &node, "Bip01");
        focus[0] = node.x;
        focus[1] = node.y;
        focus[2] = node.z;
        focus[3] = node.w;
        attacker = bakugan->meleeHitAttacker;
        attackerValue = 0;
        if (attacker != NULL) {
            entry = &((const VtblEntry *)attacker->base.base.vtable)[20];
            attackerValue = ((s32 (*)(void *))entry->fn)((u8 *)attacker + entry->delta);
        }
        effect = (GfxEffect *)GfxEffectSpawnAttached(g_worldEffectMgr, 0x82, self->camera.target);
        effect->textureSlot = attackerValue;
        BtlBakuganPlayMotion(0.0f, bakugan, 0xee, 0, 1);
        if (attacker != NULL) {
            entry = &((const VtblEntry *)attacker->base.base.vtable)[11];
            if (((s32 (*)(void *))entry->fn)((u8 *)attacker + entry->delta) != 0) {
                attacker = NULL;
            }
            if (attacker != NULL) {
                frame = GfxModelMotionFrame(&attacker->base) - 2.0f;
                /* vmin with S733 (1), then vmax with S713 (0); a tie keeps the bank operand */
                frame = frame < 1.0f ? frame : 1.0f;
                frame = frame > 0.0f ? frame : 0.0f;
                self->attackerFrame = frame;
            }
        }
    } else if (self->farCamera == 1) {
        focus[0] = ((BtlBakugan *)self->unit)->base.pos[0];
        focus[1] = ((BtlBakugan *)self->unit)->base.pos[1];
        focus[2] = ((BtlBakugan *)self->unit)->base.pos[2];
        focus[3] = ((BtlBakugan *)self->unit)->base.pos[3];
        focus[1] = focus[1] + 100.0f;
    }
    /* other modes leave focus uninitialised, as the binary does.
       eye.xyz = focus + camDir * camDistance, eye.w = focus.w; target = focus */
    dist = self->camDistance;
    self->camera.eye[0] = focus[0] + self->camDir[0] * dist;
    self->camera.eye[1] = focus[1] + self->camDir[1] * dist;
    self->camera.eye[2] = focus[2] + self->camDir[2] * dist;
    self->camera.eye[3] = focus[3];
    self->camera.target[0] = focus[0];
    self->camera.target[1] = focus[1];
    self->camera.target[2] = focus[2];
    self->camera.target[3] = focus[3];
    GfxCameraUpdate(&self->camera, 0xffffffff);
    self->flag2f0 = 0;
    self->savedCamera = g_gfxActiveCamera;
    g_gfxActiveCamera = &self->camera;
    return self;
}
