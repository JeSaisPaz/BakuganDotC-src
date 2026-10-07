// bdc 0x08875f1c BtlBakuganState17Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 17 (`+0x140`), vtable slot `+0x158` called through
   `BtlBakuganRunState`: fires the selected special art. Stores the art's motion-set index in
   `attackIndex`; goes to state 0xb when the art kind is 2 or the motion set has flag 0x20000,
   otherwise to state 8 and spawns the attached effect `0x87 + virtual slot 20()` at the anchor
   position, tagged with the unit. Art kinds 3..4 clear the timed statuses. For index 0x3f:
   kind 0x14 sets state flag 0x80000, kind 0x12 applies status 10 for 300 frames, kind 0xc
   applies status 9 for 600 frames and drops the links to this unit. Then sets state flag 0x100,
   clears flag 0x400 and, for the local player, focuses the camera (0x5a, {0,0,0,0.6}) and,
   outside state 0xb and unless the set has flag 0x200, starts a close-up (0.0, 0.2 or 0.7 with
   flag 0x800, 0x2d frames / 0x37 for kind 9 / 0x41 with flag 0x400) with close-up distance 250
   (flag 0x1000) or 350 (flag 0x2000). Finally activates the art, plays attack motion phase 0,
   calls virtual slot 6 with 1.0, for a player unit resets the motion time scale and the camera
   flash target, zeroes `dashSpeed` and zeroes `velocity` (the VFPU bank constant C720). */
void BtlBakuganState17Update(BtlBakugan *self)
{
    float focus[4] __attribute__((aligned(16)));
    BtlCombatState *combat;
    BtlAttackMotionSet *set;
    const VtblEntry *entry;
    GfxEffectMgr *mgr;
    GfxEffect *effect;
    s32 kind;
    s32 frames;
    float param;
    u32 setFlags;
    bool skip;

    combat = &self->combat;
    self->attackIndex = BtlCombatGetSelectedArtMotion(combat);
    set = (BtlAttackMotionSet *)self->attackMotions[self->attackIndex];
    if (BtlCombatGetSelectedArtKind(combat) == 2 || (set->flags & 0x20000) != 0) {
        BtlBakuganSetState(self, 0xb, 0);
    } else {
        BtlBakuganSetState(self, 8, 0);
        mgr = g_worldEffectMgr;
        entry = &((const VtblEntry *)self->base.base.vtable)[20];
        kind = ((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta);
        effect = (GfxEffect *)GfxEffectSpawnAttached(mgr, kind + 0x87, self->anchorMatrix[3]);
        effect->ownerBakugan = self;
        if (self != NULL) {
            effect->ownerId = self->base.base.id;
        }
    }
    kind = BtlCombatGetSelectedArtKind(combat);
    if (kind >= 3 && kind < 5) {
        BtlCombatClearTimedStatuses(combat);
    }
    switch (self->base.base.unk08) {
    case 0x14:
        if (self->attackIndex == 0x3f) {
            self->stateFlags |= 0x80000;
        }
        break;
    case 0x12:
        if (self->attackIndex == 0x3f) {
            BtlCombatApplyStatus(combat, 10, 300);
        }
        break;
    case 0xc:
        if (self->attackIndex == 0x3f) {
            BtlCombatApplyStatus(combat, 9, 600);
            BtlBakuganReleaseLinksTo(self);
        }
        break;
    }
    self->stateFlags |= 0x100;
    self->flags &= ~0x400u;
    if (BtlBakuganIsLocalPlayer(self)) {
        focus[0] = 0.0f;
        focus[1] = 0.0f;
        focus[2] = 0.0f;
        focus[3] = 0.600000024f;
        BtlMainSetFocusPointGlobal(0x5a, focus);
        if (self->state != 0xb) {
            frames = 0x2d;
            if (self->base.base.unk08 == 9) {
                frames = 0x37;
            }
            skip = false;
            param = 0.200000003f;
            if (set != NULL) {
                setFlags = set->flags;
                if ((setFlags & 0x200) != 0) {
                    skip = true;
                }
                if ((setFlags & 0x400) != 0) {
                    frames = 0x41;
                }
                if ((setFlags & 0x800) != 0) {
                    param = 0.699999988f;
                }
            }
            if (!skip) {
                BtlStartCameraCloseUp(0.0f, param, frames);
                if (set != NULL) {
                    if ((set->flags & 0x1000) != 0) {
                        BtlSetCloseUpDistance(250.0f);
                    } else if ((set->flags & 0x2000) != 0) {
                        BtlSetCloseUpDistance(350.0f);
                    }
                }
            }
        }
    }
    BtlCombatActivateSelectedArt(combat);
    BtlBakuganPlayAttackMotion(self, 0);
    entry = &((const VtblEntry *)self->base.base.vtable)[6];
    ((void (*)(void *, float))entry->fn)((u8 *)self + entry->delta, 1.0f);
    if (self->isPlayer != 0) {
        GfxSetMotionTimeScale(1.0f);
        ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.0f;
    }
    self->dashSpeed = 0.0f;
    self->base.velocity[0] = 0.0f;
    self->base.velocity[1] = 0.0f;
    self->base.velocity[2] = 0.0f;
    self->base.velocity[3] = 0.0f;
}
