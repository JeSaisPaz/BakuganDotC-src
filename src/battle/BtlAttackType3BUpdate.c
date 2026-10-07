// bdc 0x0887f988 BtlAttackType3BUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x3b (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   on the first frame (`age == 0`) it rotates `vel` about the Y axis by -pi/6 (side variant
   `paramF2 == 1`) or +pi/6 (`paramF2 == 2`) (x' = x*cos - z*sin, z' = x*sin + z*cos; y and w
   kept), and copies `vel` into its effect's `dir` (other variants copy it unrotated). Every frame
   then runs `BtlAttackUpdateBeam``(radius * 0.2, self, 0xcb, 0xcc, 0xcd, 0x5e, 0x28)`. */
void BtlAttackType3BUpdate(BtlAttack *self)
{
    float angle, c, s, x, y, z;
    float *dir;

    if (self->age == 0) {
        if (self->paramF2 == 1.0f || self->paramF2 == 2.0f) {
            angle = self->paramF2 == 1.0f ? -0.523598790f : 0.523598790f;
            /* vmul.s by the bank's 2/pi then vrot: cos/sin of the angle in radians */
            c = __builtin_cosf(angle);
            s = __builtin_sinf(angle);
            x = self->vel[0];
            y = self->vel[1];
            z = self->vel[2];
            self->vel[0] = x * c + y * 0.0f + z * -s;
            self->vel[2] = x * s + y * 0.0f + z * c;
        }
        dir = ((GfxEffect *)self->effect)->dir;
        dir[0] = self->vel[0];
        dir[1] = self->vel[1];
        dir[2] = self->vel[2];
        dir[3] = self->vel[3];
    }
    BtlAttackUpdateBeam(self->radius * 0.200000003f, self, 0xcb, 0xcc, 0xcd, 0x5e, 0x28);
}
