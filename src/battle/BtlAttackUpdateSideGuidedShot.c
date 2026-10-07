// bdc 0x08880118 BtlAttackUpdateSideGuidedShot
#include "bdc.h"

/* Shared update of attack types 0x27 and 0x8c (`BtlAttackType27Update` passes `hitKind` 0x4a,
   `BtlAttackType8CUpdate` 0xaf). On the first frame (`age` 0) it stores `hitKind` in `param0`,
   raises `pos[1]` by 50, turns the variant in `paramF2` into the speed (variant 2: speed 0 and
   `param0` = 6; odd variants -30, other even ones +30, stored back in `paramF2`) and sets `param1`
   = 0x9b. Every frame it then runs `BtlAttackUpdateGuided` with turn 0.1, speed `paramF2`, hit
   kind `param0`, hit effect `param1` and cancel effect 0x1a (its result is discarded). */
void BtlAttackUpdateSideGuidedShot(BtlAttack *self, s32 hitKind)
{
    float speed = self->paramF2;
    s32 variant;

    if (self->age == 0) {
        variant = (s32)speed;
        self->param0 = hitKind;
        self->pos[1] = self->pos[1] + 50.0f;
        if (variant == 2) {
            speed = 0.0f;
            self->param0 = 6;
        } else if ((variant & 1) != 0) {
            speed = -30.0f;
        } else {
            speed = 30.0f;
        }
        self->paramF2 = speed;
        self->param1 = 0x9b;
    }
    BtlAttackUpdateGuided(0.100000001f, speed, self, self->param0, self->param1, 0x1a);
}
