// bdc 0x0885d6e4 BtlUnitMode4UpdateRetreat
#include "bdc.h"

/* Scripted retreat of the mode-4 unit (BtlUnitMode4Ctor; run every frame by BtlUnitMode4Update), a
   step machine on `retreatStep` (0 = idle) started by BtlUnitMode4StartRetreat, with the frame
   counter `stepTimer`. Any active step first drops the unit's link (BtlBakuganClearLink). 1 resets
   the unit state (BtlBakuganSetState 0) and sets the input's allowed actions to 0x207; 2 (resets
   the timer) / 3 pull the AI pad's stick back and press dash (BtlAiPadStickBack,
   BtlAiPadPressDash) for 3 frames, then BtlAiPadPressNone and step 4; 4 presses dash 13 frames,
   then BtlAiPadPressNone (on the AI without a NULL check) and step 10; 5 holds action 4
   (BtlAiPadPress04) for 5 frames, then step 10; 10 enters state 0xf with dash heading -pi/2 and
   falls into 11, which runs toward `retreatPoint` (BtlUnitMode4MoveTowardPoint) and moves to step
   0x14 once within 150 units; 0x14 (resets the timer) / 0x15 press nothing, clear the allowed
   actions and face heading pi/2 (BtlBakuganSetHeading) for 3 frames (only while an AI exists),
   then step 100; 100 returns to state 0, zeroes the velocity, sets
   `arrived` and ends (step 0). Steps 6..9, 12..0x13, other values: nothing. */

void BtlUnitMode4UpdateRetreat(BtlUnitMode4 *self)
{
    BtlBakugan *unit = &self->base;
    float point[4] BDC_ALIGN16;
    float dx, dy, dz;
    float dist;
    s32 t;

    if (self->retreatStep != 0) {
        BtlBakuganClearLink(unit);
    }
    switch (self->retreatStep) {
    case 1:
        BtlBakuganSetState(unit, 0, 0);
        unit->input->allowedActions = 0x207;
        self->retreatStep = self->retreatStep + 1;
        break;
    case 2:
        self->stepTimer = 0;
        self->retreatStep = self->retreatStep + 1;
        /* fall through */
    case 3:
        if (self->ai != NULL) {
            BtlAiPadStickBack(&self->ai->pad);
            BtlAiPadPressDash(&self->ai->pad);
        }
        t = self->stepTimer;
        self->stepTimer = t + 1;
        if (t >= 2) {
            if (self->ai != NULL) {
                BtlAiPadPressNone(&self->ai->pad);
            }
            self->stepTimer = 0;
            self->retreatStep = self->retreatStep + 1;
        }
        break;
    case 4:
        if (self->ai != NULL) {
            BtlAiPadPressDash(&self->ai->pad);
        }
        t = self->stepTimer;
        self->stepTimer = t + 1;
        if (t >= 13) {
            BtlAiPadPressNone(&self->ai->pad);
            self->stepTimer = 0;
            self->retreatStep = 10;
        }
        break;
    case 5:
        if (self->ai != NULL) {
            BtlAiPadPress04(&self->ai->pad);
        }
        t = self->stepTimer;
        self->stepTimer = t + 1;
        if (t >= 5) {
            self->stepTimer = 0;
            self->retreatStep = 10;
        }
        break;
    case 10:
        BtlBakuganSetState(unit, 0xf, 0);
        unit->dashHeading = -1.57079637f;
        self->retreatStep = self->retreatStep + 1;
        /* fall through */
    case 11:
        /* point = retreatPoint (quad copy) */
        point[0] = self->retreatPoint[0];
        point[1] = self->retreatPoint[1];
        point[2] = self->retreatPoint[2];
        point[3] = self->retreatPoint[3];
        BtlUnitMode4MoveTowardPoint(self, point);
        /* dist = |pos.xyz - retreatPoint.xyz| */
        dx = unit->base.pos[0] - self->retreatPoint[0];
        dy = unit->base.pos[1] - self->retreatPoint[1];
        dz = unit->base.pos[2] - self->retreatPoint[2];
        dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
        if (dist < 150.0f) {
            self->retreatStep = 0x14;
        }
        break;
    case 0x14:
        self->stepTimer = 0;
        self->retreatStep = self->retreatStep + 1;
        /* fall through */
    case 0x15:
        if (self->ai != NULL) {
            BtlAiPadPressNone(&self->ai->pad);
            unit->input->allowedActions = 0;
            BtlBakuganSetHeading(unit, 1.57079637f);
        }
        t = self->stepTimer;
        self->stepTimer = t + 1;
        if (t >= 2) {
            self->stepTimer = 0;
            self->retreatStep = 100;
        }
        break;
    case 100:
        BtlBakuganSetState(unit, 0, 0);
        /* velocity = (0, 0, 0, 0) (bank C720) */
        unit->base.velocity[0] = 0.0f;
        unit->base.velocity[1] = 0.0f;
        unit->base.velocity[2] = 0.0f;
        unit->base.velocity[3] = 0.0f;
        self->arrived = 1;
        self->retreatStep = 0;
        break;
    default:
        break;
    }
}
