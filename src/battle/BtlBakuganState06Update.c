// bdc 0x08874bf4 BtlBakuganState06Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 6 (`+0x140`), vtable slot `+0x100` called through
   `BtlBakuganRunState`: the knock-out sequence, stepped by `subTimer` (any other value than
   0..4 does nothing).
   0: zeroes `velocity` (bank C720), holds gravity (`gravityHold` 999, `gravity` 0),
      sets collider flags (both |= 7 when present, then collider0 |= 1|4 with `hitTimer` 0,
      collider1 |= 0x40|4), updates the shadow, plays sound 0x200011, stops every world effect
      attached to the anchor position or `effectAnchor`, spawns the attached effect
      `0x70 + virtual slot 20()` at the anchor position, focuses the camera (0x1e, {0,0,0,0.5}),
      drops an item only when the camera task exists and `BtlMainCheckWin` returns 0, sets
      `subWait` 0x19 and falls into 1.
   1: fade: t = clamp((0x23 - subWait) * 0.03, 0, 1), c = cos(t*pi) (`vcos.s` of t*pi*2/pi);
      sets the fog colour (virtual slot 4) to the
      attribute colour with alpha (1 - c) / 2, scales the Bip01_Pelvis node's xyz scale by
      1 - (1 - c) / 2 * 0.3 (its `scaleW` set to 0) and lowers `ambient[3]` by 0.05.
      While `subWait` (post-decremented) was > 0 it returns; then disables fog, resets the pelvis
      scale to {1,1,1,0}, clears `ambient[3]` and falls into 2.
   2: clears `ambient[3]`, `subWait` = 300. In battle rule mode 1 (script global 8): a non-player
      CPU unit (virtual slot 13) advances to 3, any other unit parks at 999. Otherwise profile
      word 7: 1..2 enters state 0x10 (revive), 0 parks at 999, else stays in 2.
   3: waits for `subWait` (post-decremented) to run out, then falls into 4.
   4: advances `subTimer` and queues the object for deletion (`CoreObjectDeferDelete`). */
void BtlBakuganState06Update(BtlBakugan *self)
{
    float focus[4];
    float colour[4];
    const VtblEntry *entry;
    CollisionCollider *collider;
    GfxEffectMgr *mgr;
    GmoNode *node;
    float *srcColour;
    float t;
    float angle;
    float c;
    float scale;
    s32 kind;
    s32 wait;
    s32 next;
    s32 word;
    bool advance;

    switch (self->subTimer) {
    case 0:
        /* velocity = C720 (bank zero vector) */
        self->base.velocity[0] = 0.0f;
        self->base.velocity[1] = 0.0f;
        self->base.velocity[2] = 0.0f;
        self->base.velocity[3] = 0.0f;
        self->gravityHold = 999;
        self->gravity = 0.0f;
        if (self->collider0 != NULL) {
            self->collider0->flags |= 7;
        }
        if (self->collider1 != NULL) {
            self->collider1->flags |= 7;
        }
        collider = self->collider0;
        collider->hitTimer = 0;
        collider->flags |= 1;
        self->collider0->flags |= 4;
        self->collider1->flags |= 0x40;
        self->collider1->flags |= 4;
        BtlShadowUpdate(self->shadow);
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0x200011, 0, 0);
        }
        GfxEffectStopAttached(g_worldEffectMgr, -1, self->anchorMatrix[3]);
        GfxEffectStopAttached(g_worldEffectMgr, -1, self->effectAnchor);
        mgr = g_worldEffectMgr;
        entry = &((const VtblEntry *)self->base.base.vtable)[20];
        kind = ((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta);
        GfxEffectSpawnAttached(mgr, kind + 0x70, self->anchorMatrix[3]);
        focus[0] = 0.0f;
        focus[1] = 0.0f;
        focus[2] = 0.0f;
        focus[3] = 0.5f;
        BtlMainSetFocusPointGlobal(0x1e, focus);
        if (BtlCameraTaskExists() && BtlMainCheckWin((BtlMain *)BtlGetCameraTask()) == 0) {
            BtlBakuganDropItem(self);
        }
        self->subWait = 0x19;
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 1:
        srcColour = BtlBakuganGetAttributeColor(self);
        colour[0] = srcColour[0];
        colour[1] = srcColour[1];
        colour[2] = srcColour[2];
        colour[3] = srcColour[3];
        t = (float)(0x23 - self->subWait) * 0.0299999993f;
        /* vmin.s with S733 (1), then vmax.s with S713 (0); t is never NaN */
        t = t < 1.0f ? t : 1.0f;
        t = t > 0.0f ? t : 0.0f;
        angle = t * 3.14159274f;
        /* vcos.s of angle * S703 (2/pi): cos in radians */
        c = __builtin_cosf(angle);
        colour[3] = (1.0f - c) * 0.5f;
        entry = &((const VtblEntry *)self->base.base.vtable)[4];
        ((void (*)(void *, const float *))entry->fn)((u8 *)self + entry->delta, colour);
        c = __builtin_cosf(angle);
        scale = 1.0f - (1.0f - c) * 0.5f * 0.300000012f;
        node = (GmoNode *)GfxModelFindNode(&self->base, "Bip01_Pelvis");
        /* vscl.t into C710, stored with sv.q: lane 3 is the bank S713 (0) */
        node->scale[0] = node->scale[0] * scale;
        node->scale[1] = node->scale[1] * scale;
        node->scale[2] = node->scale[2] * scale;
        node->scaleW = 0.0f;
        self->base.ambient[3] = self->base.ambient[3] - 0.0500000007f;
        wait = self->subWait;
        self->subWait = wait - 1;
        if (wait > 0) {
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
    case 2:
        self->base.ambient[3] = 0.0f;
        self->subWait = 300;
        if (g_scriptGlobalVars[8] == 1) {
            advance = false;
            if (self->isPlayer == 0) {
                entry = &((const VtblEntry *)self->base.base.vtable)[13];
                if (((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta) != 0) {
                    advance = true;
                }
            }
            next = 999;
            if (advance) {
                next = self->subTimer + 1;
            }
            self->subTimer = next;
        } else {
            word = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
            if (word > 0) {
                if (word < 3) {
                    BtlBakuganSetState(self, 0x10, 0);
                }
            } else if (word >= 0) {
                self->subTimer = 999;
            }
        }
        return;
    case 3:
        wait = self->subWait;
        self->subWait = wait - 1;
        if (wait > 0) {
            return;
        }
        self->subTimer = self->subTimer + 1;
        /* fallthrough */
    case 4:
        self->subTimer = self->subTimer + 1;
        CoreObjectDeferDelete((CoreObject *)self, 0);
        return;
    default:
        return;
    }
}
