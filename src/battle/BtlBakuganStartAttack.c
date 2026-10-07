// bdc 0x0886b870 BtlBakuganStartAttack
#include "bdc.h"

/* Attack-command entry of `BtlBakuganSetState`: resets `attackIndex`/`comboIndex` to -1 and picks
   the attack from the frame's `commands` and `cmd`:
   - `0x20`: state 0x11 and `flags |= 0x20`;
   - `0x800`: attack 0x1f (0x21 airborne) shifted by the target's elevation class and distance
     (AddRangedElevationShift, kind 7), state 8 and `stateFlags |= 0x80000`;
   - `cmd == 2` without `0x20000`, or `0x4` without `0x20000`: combo 4 (5 with `0x10000`), state 9
     keeping the motion;
   - `cmd == 2` with `0x20000`: attack 0x23/0x24 while playing motion 7/6, else 0x22 (0x20 when the
     target direction is 0), +10/+20 by elevation, +3 airborne; state 10 keeping the motion if that
     attack has motions and the direction was not 0, else attack 0x1e (0x20 airborne) +10/+20 by
     elevation in state 8;
   - other `0x20000`: attack 0x1e (0x20 airborne) shifted by elevation and distance
     (AddRangedElevationShift, kind 2), state 8;
   - otherwise a ground combo: 2/3 when airborne or the airborne target is more than 60 above,
     else 0/1 (+1 with `0x10000`).
   Then resets the attack-step fields, latches the target in `attackObj`, keeps `dashSpeed` only in
   state 10, starts the combo step (`BtlBakuganPlayComboStepMotion`, motion speed
   `comboMotionSpeed` for combos 0/1) or attack (`BtlBakuganPlayAttackMotion`, not in state 0x11)
   motion, and counts the attack (`BtlBakuganCountAttackStat`). */

/* Horizontal (XZ) distance between two positions (VFPU vsub.q, y zeroed, vdot.t, vsqrt.s). */
static float HorizontalDistance(const float *a, const float *b)
{
    float dx = a[0] - b[0];
    float dz = a[2] - b[2];

    return __builtin_sqrtf(dx * dx + dz * dz);
}

/* Elevation shift of a ranged attack (commands 0x800 / 0x20000): +10 for class 4; +20 for class
   5 unless this unit's kind (`unk08`) is `farKind` and the target is not within 3000 (also NaN);
   +10 for the other classes only in that far case. */
static void AddRangedElevationShift(BtlBakugan *self, u32 farKind)
{
    BtlBakugan *target;
    int elev;
    float dist;

    if (BtlBakuganGetTarget(self) == NULL) {
        return;
    }
    elev = BtlBakuganClassifyElevationToObject(self, BtlBakuganGetTarget(self));
    target = BtlBakuganGetTarget(self);
    dist = HorizontalDistance(self->base.pos, target->base.pos);
    if (elev == 4) {
        self->attackIndex += 10;
    } else if (elev == 5) {
        if (self->base.base.unk08 != farKind || dist <= 3000.0f) {
            self->attackIndex += 20;
        }
    } else if (self->base.base.unk08 == farKind && !(dist <= 3000.0f)) {
        self->attackIndex += 10;
    }
}

/* Elevation shift of a melee attack: +10 for class 4, +20 for class 5. */
static void AddMeleeElevationShift(BtlBakugan *self)
{
    int elev;

    if (BtlBakuganGetTarget(self) == NULL) {
        return;
    }
    elev = BtlBakuganClassifyElevationToObject(self, BtlBakuganGetTarget(self));
    if (elev == 4) {
        self->attackIndex += 10;
    } else if (elev == 5) {
        self->attackIndex += 20;
    }
}

void BtlBakuganStartAttack(BtlBakugan *self, int cmd)
{
    float speed;
    bool fallback;
    bool targetHigh;
    BtlBakugan *target;
    float dy;
    const VtblEntry *entry;

    speed = self->dashSpeed;
    self->attackIndex = -1;
    self->comboIndex = -1;
    if ((self->commands & 0x20) != 0) {
        BtlBakuganSetState(self, 0x11, 0);
        self->flags |= 0x20;
    } else if ((self->commands & 0x800) != 0) {
        self->attackIndex = BtlBakuganIsAirborne(self, 0) ? 0x21 : 0x1f;
        AddRangedElevationShift(self, 7);
        BtlBakuganSetState(self, 8, 0);
        self->stateFlags |= 0x80000;
    } else if (cmd == 2) {
        if ((self->commands & 0x20000) == 0) {
            self->comboIndex = (self->commands & 0x10000) ? 5 : 4;
            BtlBakuganSetState(self, 9, 1);
        } else {
            fallback = false;
            if (BtlBakuganIsMotion(self, 7)) {
                self->attackIndex = 0x23;
            } else if (BtlBakuganIsMotion(self, 6)) {
                self->attackIndex = 0x24;
            } else {
                self->attackIndex = 0x22;
                if (BtlBakuganGetTargetDirection(self) == 0) {
                    self->attackIndex = 0x20;
                    fallback = true;
                }
            }
            AddMeleeElevationShift(self);
            if (BtlBakuganIsAirborne(self, 1) && !fallback) {
                self->attackIndex += 3;
            }
            if (self->attackMotions[self->attackIndex] == NULL || fallback) {
                self->attackIndex = BtlBakuganIsAirborne(self, 1) ? 0x20 : 0x1e;
                AddMeleeElevationShift(self);
                BtlBakuganSetState(self, 8, 0);
            } else {
                BtlBakuganSetState(self, 10, 1);
            }
        }
    } else if ((self->commands & 0x20000) == 0) {
        if ((self->commands & 4) != 0) {
            self->comboIndex = (self->commands & 0x10000) ? 5 : 4;
            BtlBakuganSetState(self, 9, 1);
        } else {
            targetHigh = false;
            if (BtlBakuganGetTarget(self) != NULL) {
                target = BtlBakuganGetTarget(self);
                dy = target->base.pos[1] - self->base.pos[1];
                if (BtlBakuganIsAirborne(BtlBakuganGetTarget(self), 1) && !(dy <= 60.0f)) {
                    targetHigh = true;
                }
            }
            if (BtlBakuganIsAirborne(self, 1) | targetHigh) {
                self->comboIndex = (self->commands & 0x10000) ? 3 : 2;
            } else {
                self->comboIndex = (self->commands & 0x10000) != 0;
            }
        }
    } else {
        self->attackIndex = BtlBakuganIsAirborne(self, 1) ? 0x20 : 0x1e;
        AddRangedElevationShift(self, 2);
        BtlBakuganSetState(self, 8, 0);
    }

    self->stateFlags |= 0x400000;
    self->flags &= ~4u;
    self->queuedComboSteps = 0;
    self->attackFrame = 0;
    self->finalStepPlaying = 0;
    self->foldAttackIds = 0;
    self->attackSpawnBlocked = 0;
    self->comboInputs[0] = (self->commands & 0x10000) ? 2 : 1;
    self->comboInputCount = 1;
    self->comboStep = 0;
    self->comboAirVariant = 0;
    self->motionHitLanded = 0;
    self->attackObj = BtlBakuganGetTarget(self);
    if (self->state != 10) {
        speed = 0.0f;
    }
    self->dashSpeed = speed;
    self->attackEventFrame = 0;
    self->hitCount = 0;
    if (self->comboIndex != -1) {
        BtlBakuganPlayComboStepMotion(self, 0);
        if (self->comboIndex == 0 || self->comboIndex == 1) {
            entry = &((const VtblEntry *)self->base.base.vtable)[6];
            ((float (*)(float, void *))entry->fn)(self->combat.stats->comboMotionSpeed,
                                                  (u8 *)self + entry->delta);
        }
    } else if (self->state != 0x11) {
        BtlBakuganPlayAttackMotion(self, 0);
    }
    BtlBakuganCountAttackStat(self);
}
