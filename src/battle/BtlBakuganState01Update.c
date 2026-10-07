// bdc 0x088718d4 BtlBakuganState01Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 1 (the walk), run through `BtlBakuganRunState`.
   It turns toward its target (`BtlBakuganTurnTowardTarget`). Guard: with command bit 8 or a
   running block timer, energy, at least one frame in the state and not airborne, it sets state
   flag 0x200000, pays the guard energy (`BtlBakuganApplyStateEnergy` state 7), sets the
   regeneration delay to 15 and starts the guard effect; otherwise it ends the guard effect.
   Without move command bit 1 it returns to state 0. Otherwise: facing a target, it plays the
   directional walk motion `1 + ``BtlBakuganGetTargetDirSlot` (switching motions keeps the frame,
   clamped to the new motion's end) at full speed; with no target it plays motion 1, turns toward
   the input heading (rate 0.3) and scales the speed by how small the remaining turn is
   (`(pi - min(|step| + 0.3 pi, pi)) * 1.5 / pi`, at most 1). The motion speed (virtual slot 6) is
   0.5 while guarding, else 0.7 when `slowWalk` is set and the attribute (virtual slot 20) is not
   2, else 1; `dashSpeed` eases by 0.2 x the motion time scale toward the walk speed (halved while
   guarding), and the horizontal velocity is `dashSpeed` x that factor along the input heading
   (VFPU vrot.q with the bank constant S703 = 2/pi). Then it applies hover and
   switches state: attack command → `BtlBakuganStartAttackOrArt`; command bit 2 with energy →
   0xc; command bit 4 with energy → 2; airborne → 0x17. Leaving state 1 resets `dashSpeed` to the
   walk speed, and leaving to any state but 0 ends the guard effect. Finally `stateFrames` is
   incremented. */

void BtlBakuganState01Update(BtlBakugan *self)
{
    const VtblEntry *entry;
    float factor;
    float frame;
    float end;
    float speed;
    float timeScale;
    bool guarding;
    bool slow;
    int faced;
    int motion;
    int state;

    faced = BtlBakuganTurnTowardTarget(self, NULL);
    if (((self->commands & 8) != 0 || self->blockTimer != 0) &&
        BtlCombatHasEnergy(&self->combat) != 0 && self->stateFrames != 0 &&
        BtlBakuganIsAirborne(self, 1) == 0) {
        self->stateFlags |= 0x200000;
        BtlBakuganApplyStateEnergy(self, 7, 0);
        self->combat.regenDelay = 15.0f;
        BtlBakuganStartGuardEffect(self);
    } else {
        BtlBakuganEndGuardEffect(self);
    }

    if ((self->commands & 1) == 0) {
        BtlBakuganSetState(self, 0, 0);
    } else {
        if (faced != 0) {
            motion = BtlBakuganGetTargetDirSlot(self) + 1;
            if (BtlBakuganIsMotion(self, motion) == 0) {
                frame = GfxModelMotionFrame(&self->base);
                GfxModelPlayMotion(0.200000003f, &self->base, (u16)self->motionTable[motion], 1);
                end = GfxModelGetMotionEnd(&self->base);
                if (end < frame) {
                    frame = end;
                }
                GfxModelSwapMotionFrame(&self->base, frame);
            }
            factor = 1.0f;
        } else {
            BtlBakuganPlayMotion(0.200000003f, self, 1, 1, 0);
            factor = ABS(BtlBakuganTurnToward(self->input->heading, 0.300000012f, 0.0f, self)) +
                     0.942477882f;
            if (!(factor <= 3.14159274f)) {
                factor = 3.14159274f;
            }
            factor = (3.14159274f - factor) * 0.477464825f;
            if (!(factor <= 1.0f)) {
                factor = 1.0f;
            }
        }
        guarding = (self->stateFlags & 0x200000) != 0;
        if (guarding) {
            entry = &((const VtblEntry *)self->base.base.vtable)[6];
            ((float (*)(float, void *))entry->fn)(0.5f, (u8 *)self + entry->delta);
            speed = self->dashSpeed;
            timeScale = GfxGetMotionTimeScale();
            speed = speed + (BtlBakuganGetWalkSpeed(self) * 0.5f - self->dashSpeed) *
                                0.200000003f * timeScale;
        } else {
            slow = false;
            if (self->slowWalk != 0) {
                entry = &((const VtblEntry *)self->base.base.vtable)[20];
                if (((int (*)(void *))entry->fn)((u8 *)self + entry->delta) != 2) {
                    slow = true;
                }
            }
            entry = &((const VtblEntry *)self->base.base.vtable)[6];
            if (slow) {
                ((float (*)(float, void *))entry->fn)(0.699999988f, (u8 *)self + entry->delta);
            } else {
                ((float (*)(float, void *))entry->fn)(1.0f, (u8 *)self + entry->delta);
            }
            speed = self->dashSpeed;
            timeScale = GfxGetMotionTimeScale();
            speed = speed + (BtlBakuganGetWalkSpeed(self) - self->dashSpeed) * 0.200000003f *
                                timeScale;
        }
        self->dashSpeed = speed;
        speed = speed * factor;
        /* velocity.x = cos(heading) * speed, velocity.z = sin(heading) * speed (vrot.q of
           heading x 2/pi); velocity.y is left untouched */
        self->base.velocity[0] = __builtin_cosf(self->input->heading) * speed;
        self->base.velocity[2] = __builtin_sinf(self->input->heading) * speed;
    }

    BtlBakuganApplyHover(self);
    if (BtlBakuganHasAttackCommand(self) != 0) {
        BtlBakuganStartAttackOrArt(self, 0);
    } else if ((self->commands & 2) != 0 && BtlCombatHasEnergy(&self->combat) != 0) {
        BtlBakuganSetState(self, 0xc, 0);
    } else if ((self->commands & 4) != 0 && BtlCombatHasEnergy(&self->combat) != 0) {
        BtlBakuganSetState(self, 2, 0);
    } else if (BtlBakuganIsAirborne(self, 1) != 0) {
        BtlBakuganSetState(self, 0x17, 0);
    }

    state = self->state;
    if (state != 1) {
        self->dashSpeed = BtlBakuganGetWalkSpeed(self);
        state = self->state;
    }
    if (state != 1 && state != 0) {
        BtlBakuganEndGuardEffect(self);
    }
    self->stateFrames++;
}
