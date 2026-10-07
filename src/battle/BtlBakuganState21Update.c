// bdc 0x08876248 BtlBakuganState21Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 21 (`+0x140`), vtable slot `+0x178` called through
   `BtlBakuganRunState`: the knocked-out sequence, stepped by `subTimer`. 0 stops the effects
   attached to `anchorMatrix[3]` and `effectAnchor`, disables both colliders (flags |= 7,
   `hitTimer = 0`) and waits 30 frames damping the horizontal velocity by 0.8 per frame; 2 zeroes
   the velocity, spawns world effect 0x76 at `pos` and plays motion 0xfa; 3 waits
   for 90% of it and returns to motion 0 (looped); 4/5 wait 15 frames, then 10 spawns effect
   `0x77 + attribute` (virtual slot 20) attached to `anchorMatrix[3]` with sound 0x200014; 11
   lifts the unit 3 units per frame with gravity off for 60 frames; 30 plays sound 0x200011 and,
   in knock-out mode 2, focuses the camera on the unit (`BtlCameraFocusUnit`); 31 fades the
   unit out over 25 frames (attribute colour `BtlBakuganGetAttributeColor` with a cosine alpha
   ramp handed to virtual slot 4, `"Bip01_Pelvis"` scale shrunk, ambient alpha -0.05 per frame);
   32-34 wait, raycast down from 3000 units above `pos` (`CollisionRaycastPoint`) and drop item
   kind 3 (life 600) there unless the rule mode `g_scriptGlobalVars[8] == 2` and profile word 0x1a
   is 1; 35/36 restore the default camera in mode 2; 40 sets `subWait = 300` and goes on to 41
   only for a non-player CPU unit (virtual slot 13), otherwise parks `subTimer` at 999; 41 waits
   (cut to 0 when script flag 5 is set and at least 4 units count in
   `BtlCountUnitsWithVirtual54Or64`) and 100 queues the unit for deletion
   (`CoreObjectDeferDelete`). The VFPU reads only bank constants: C720 (zero velocity), S703
   (2/pi), S713 (0, clamp floor and the pelvis `scaleW`) and S733 (1, clamp ceiling). */
void BtlBakuganState21Update(BtlBakugan *self)
{
    float tint[4];
    float ray[4];
    float t;
    float angle;
    float c;
    float scale;
    const VtblEntry *entry;
    GfxEffectMgr *mgr;
    GmoNode *node;
    float *color;
    s32 attribute;
    u8 drop;
    u8 advance;

    switch (self->subTimer) {
    case 0:
        GfxEffectStopAttached(g_worldEffectMgr, -1, self->anchorMatrix[3]);
        GfxEffectStopAttached(g_worldEffectMgr, -1, self->effectAnchor);
        if (self->collider0 != NULL) {
            self->collider0->hitTimer = 0;
            self->collider0->flags |= 1;
        }
        if (self->collider0 != NULL) {
            self->collider0->flags |= 4;
        }
        if (self->collider1 != NULL) {
            self->collider1->flags |= 0x40;
        }
        if (self->collider1 != NULL) {
            self->collider1->flags |= 4;
        }
        if (self->collider0 != NULL) {
            self->collider0->flags |= 7;
        }
        if (self->collider1 != NULL) {
            self->collider1->flags |= 7;
        }
        self->subWait = 30;
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 1:
        /* vscl.t by 0.8f (0x3f4ccccd); only lanes x and z are stored back */
        self->base.velocity[0] = self->base.velocity[0] * 0.8f;
        self->base.velocity[2] = self->base.velocity[2] * 0.8f;
        if (--self->subWait > 0) {
            return;
        }
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 2:
        /* sv.q of the bank's C720 (0, 0, 0, 0) */
        self->base.velocity[0] = 0.0f;
        self->base.velocity[1] = 0.0f;
        self->base.velocity[2] = 0.0f;
        self->base.velocity[3] = 0.0f;
        GfxEffectSpawn(g_worldEffectMgr, 0x76, self->base.pos);
        BtlBakuganPlayMotion(0.2f, self, 0xfa, 0, 0);
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 3:
        if (GfxModelMotionReached(&self->base, 0.9f)) {
            BtlBakuganPlayMotion(0.2f, self, 0, 1, 0);
            self->subTimer = self->subTimer + 1;
        }
        break;
    case 4:
        self->subWait = 15;
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 5:
        if (--self->subWait > 0) {
            return;
        }
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 6:
        self->subTimer = 10;
        break;
    case 10:
        entry = &((const VtblEntry *)self->base.base.vtable)[20];
        mgr = g_worldEffectMgr;
        attribute = ((int (*)(void *))entry->fn)((u8 *)self + entry->delta);
        GfxEffectSpawnAttached(mgr, attribute + 0x77, self->anchorMatrix[3]);
        self->subWait = 60;
        self->subTimer = self->subTimer + 1;
        BtlBakuganPlaySound(self, 0x200014, 0, 0);
        break;
    case 11:
        self->gravityHold = 999;
        self->gravity = 0.0f;
        self->base.pos[1] = self->base.pos[1] + 3.0f;
        if (--self->subWait > 0) {
            return;
        }
        self->subTimer = 30;
        /* fallthrough */
    case 30:
        /* sv.q of the bank's C720 (0, 0, 0, 0) */
        self->base.velocity[0] = 0.0f;
        self->base.velocity[1] = 0.0f;
        self->base.velocity[2] = 0.0f;
        self->base.velocity[3] = 0.0f;
        BtlShadowUpdate((void **)self->shadow);
        BtlBakuganPlaySound(self, 0x200011, 0, 0);
        if (self->knockOutMode == 2 && BtlCameraTaskExists()) {
            BtlCameraFocusUnit(500.0f, 100.0f, 40.0f, BtlGetCameraTask(), self, 0, 0, NULL);
        }
        self->subWait = 25;
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 31:
        color = BtlBakuganGetAttributeColor(self);
        tint[0] = color[0];
        tint[1] = color[1];
        tint[2] = color[2];
        tint[3] = color[3];
        t = (float)(35 - self->subWait) * 0.03f;
        /* vmin.s with the bank's S733 (1), then vmax.s with S713 (0); t is never NaN */
        if (t > 1.0f) {
            t = 1.0f;
        }
        if (t <= 0.0f) {
            t = 0.0f;
        }
        angle = t * 3.14159274f;
        /* vcos.s of angle * S703 (2/pi): the quarter turn cancels */
        c = __builtin_cosf(angle);
        tint[3] = (1.0f - c) * 0.5f;
        entry = &((const VtblEntry *)self->base.base.vtable)[4];
        ((void (*)(void *, float *))entry->fn)((u8 *)self + entry->delta, tint);
        /* the asm recomputes the same cosine after the call */
        c = __builtin_cosf(angle);
        scale = 1.0f - (1.0f - c) * 0.5f * 0.3f;
        node = (GmoNode *)GfxModelFindNode(&self->base, "Bip01_Pelvis");
        /* vscl.t of the xyz lanes, then sv.q puts the bank's S713 (0) in scaleW */
        node->scale[0] = node->scale[0] * scale;
        node->scale[1] = node->scale[1] * scale;
        node->scale[2] = node->scale[2] * scale;
        node->scaleW = 0.0f;
        self->base.ambient[3] = self->base.ambient[3] - 0.05f;
        if (self->subWait-- > 0) {
            return;
        }
        self->base.fogEnabled = 0;
        self->subTimer = self->subTimer + 1;
        node = (GmoNode *)GfxModelFindNode(&self->base, "Bip01_Pelvis");
        node->scale[0] = 1.0f;
        node->scale[1] = 1.0f;
        node->scale[2] = 1.0f;
        node->scaleW = 0.0f;
        self->base.ambient[3] = 0.0f;
        /* fallthrough */
    case 32:
        self->subWait = (self->knockOutMode == 2) ? 40 : 0;
        self->subTimer = self->subTimer + 1;
        break;
    case 33:
        if (--self->subWait > 0) {
            return;
        }
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 34:
        if (self->knockOutMode == 2) {
            self->base.pos[1] = self->base.pos[1] - 250.0f;
            BtlCameraFocusUnit(450.0f, 150.0f, 40.0f, BtlGetCameraTask(), self, 0, 0, NULL);
            self->subWait = 45;
        } else {
            self->subWait = 0;
        }
        drop = 1;
        ray[0] = self->base.pos[0];
        ray[1] = self->base.pos[1];
        ray[2] = self->base.pos[2];
        ray[3] = self->base.pos[3];
        ray[1] = ray[1] + 3000.0f;
        CollisionRaycastPoint(ray, ray);
        if (g_scriptGlobalVars[8] == 2 && SaveProfileGetWord(SaveGetProfile(), 0x1a) == 1) {
            drop = 0;
        }
        if (drop) {
            BtlItemCreate(3, (u32 *)ray, 600, 1, NULL);
        }
        self->subTimer = self->subTimer + 1;
        break;
    case 35:
        if (--self->subWait > 0) {
            return;
        }
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 36:
        if (self->knockOutMode == 2) {
            self->koCameraDone = 1;
            if (BtlCameraTaskExists()) {
                BtlCameraEnterDefaultMode(BtlGetCameraTask());
            }
        }
        self->subTimer = 40;
        /* fallthrough */
    case 40:
        self->subWait = 300;
        self->base.ambient[3] = 0.0f;
        advance = 0;
        if (self->isPlayer == 0) {
            entry = &((const VtblEntry *)self->base.base.vtable)[13];
            if (((int (*)(void *))entry->fn)((u8 *)self + entry->delta) != 0) {
                advance = 1;
            }
        }
        self->subTimer = advance ? self->subTimer + 1 : 999;
        break;
    case 41:
        if (CoreBitsetTest(5, g_scriptGlobalBits) && BtlCountUnitsWithVirtual54Or64() >= 4) {
            self->subWait = 0;
        }
        if (self->subWait-- > 0) {
            return;
        }
        self->subTimer = 100;
        /* fallthrough */
    case 100:
        self->subTimer = self->subTimer + 1;
        CoreObjectDeferDelete(&self->base.base, 0);
        break;
    default:
        break;
    }
}
