// bdc 0x088fed90 BtlDemoPlaceBakugan
#include "bdc.h"

/* Places a Bakugan unit for the demo: clears flag bits 0x46 of its collider `collider1` (when set),
   sets its heading to `pos[3]` (`BtlBakuganSetHeading`), copies the 16-byte `pos` into
   `groundPoint`, `base.pos` and the model root matrix translation (`base.data->rootMatrix[12..15]`)
   and zeroes `base.velocity` (VFPU bank C720 = 0), plays motion 0 looped with blend 0 three times (`GfxModelPlayMotion`), resets
   it to state 0 (`BtlBakuganSetState`), clears `gravityHold` and calls its vtable entry 7. */

void BtlDemoPlaceBakugan(BtlDemo *demo, BtlBakugan *unit, const float *pos)
{
    const VtblEntry *e;

    (void)demo;
    if (unit->collider1 != NULL) {
        unit->collider1->flags &= ~0x46u;
    }
    BtlBakuganSetHeading(unit, pos[3]);
    {
        int k;

        for (k = 0; k < 4; k++) {
            unit->groundPoint[k] = pos[k];
        }
        for (k = 0; k < 4; k++) {
            unit->base.pos[k] = pos[k];
        }
        for (k = 0; k < 4; k++) {
            unit->base.velocity[k] = 0.0f;
        }
        for (k = 0; k < 4; k++) {
            unit->base.data->rootMatrix[12 + k] = pos[k];
        }
    }
    GfxModelPlayMotion(0.0f, &unit->base, 0, 1);
    GfxModelPlayMotion(0.0f, &unit->base, 0, 1);
    GfxModelPlayMotion(0.0f, &unit->base, 0, 1);
    BtlBakuganSetState(unit, 0, 0);
    unit->gravityHold = 0;
    e = &((const VtblEntry *)unit->base.base.vtable)[7];
    ((void (*)(void *))e->fn)((u8 *)unit + e->delta);
}
