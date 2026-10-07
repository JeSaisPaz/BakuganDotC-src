// bdc 0x08872914 BtlBakuganState03Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 3, run through `BtlBakuganRunState`. Sets state flag
   0x4000000 and counts `stateCounter` up. Charging (state flag 0x100000 was set): while motion 0xff
   loops, `subTimer` counts down (30 more with command 0x8000; `statusTimer` keeps the previous
   value) and at < 1 status 1 ends (`BtlBakuganEndStatus`) and the state finishes; before that,
   at 90% of the lead-in motion it starts looping motion 0xff, plays sound 0x2001f5 for the local
   player and spawns charge effect 0x44 (0x45 with flag 1) attached to the anchor position; when
   airborne the vertical speed takes the stat table's `gravity`. Otherwise at 85% of the motion it
   replays motion 0xff up to twice while flag 0x800 is set, else clears 0x800 and finishes. Then
   hover, footstep dust while grounded and moving (squared speed > 256; foot by `stateCounter`
   parity), and when finished clears 0x100000, resets the counter window and goes to state 0 unless
   `BtlBakuganHandleIdleCommands` switched. Finally damps velocity x/z by
   `BtlScaleRetentionByTimeStep`(0.85). */
void BtlBakuganState03Update(BtlBakugan *self)
{
    float *vel = self->base.velocity;
    u32 prevStateFlags = self->stateFlags;
    s32 finished = 0;
    float speedSq;
    float keep;

    self->stateFlags = prevStateFlags | 0x4000000;
    self->stateCounter++;

    if ((prevStateFlags & 0x100000) != 0) {
        if (BtlBakuganIsMotion(self, 0xff)) {
            self->statusTimer = self->subTimer;
            self->subTimer--;
            if ((self->commands & 0x8000) != 0) {
                self->subTimer -= 0x1e;
            }
            if (self->subTimer < 1) {
                BtlBakuganEndStatus(self, 1);
                finished = 1;
            }
        } else if (GfxModelMotionReached(&self->base, 0.899999976f)) {
            BtlBakuganPlayMotion(0.200000003f, self, 0xff, 1, 0);
            if (BtlBakuganIsLocalPlayer(self)) {
                BtlBakuganPlaySound(self, 0x2001f5, 0, 0);
            }
            if ((self->flags & 1) != 0) {
                GfxEffectUpdateNow(GfxEffectSpawnAttached(g_worldEffectMgr, 0x45, self->anchorMatrix[3]));
            } else {
                GfxEffectUpdateNow(GfxEffectSpawnAttached(g_worldEffectMgr, 0x44, self->anchorMatrix[3]));
            }
        }
        if (BtlBakuganIsAirborne(self, 1)) {
            self->base.velocity[1] = self->combat.stats->gravity;
        }
    } else if (GfxModelMotionReached(&self->base, 0.850000024f)) {
        if ((self->flags & 0x800) != 0 && self->subWait < 2) {
            self->subWait++;
            BtlBakuganPlayMotion(0.200000003f, self, 0xff, 1, 0);
        } else {
            self->flags &= ~0x800u;
            finished = 1;
        }
    }

    if (!BtlBakuganApplyHover(self) && !BtlBakuganIsAirborne(self, 1)) {
        /* squared length of velocity xyz */
        speedSq = vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2];
        if (!(speedSq <= 256.0f)) {
            BtlBakuganSpawnFootstepEffect(40.0f, 2.0f, self, (self->stateCounter & 1) != 0 ? 2 : 3);
        }
    }
    if (finished) {
        self->stateFlags &= ~0x100000u;
        BtlBakuganResetCounterWindow(self);
        if (!BtlBakuganHandleIdleCommands(self, 0)) {
            BtlBakuganSetState(self, 0, 0);
        }
    }
    keep = BtlScaleRetentionByTimeStep(0.850000024f);
    /* velocity.x and .z *= keep; y untouched */
    vel[0] = vel[0] * keep;
    vel[2] = vel[2] * keep;
}
