// bdc 0x0885bbfc BtlUnitAltCtor
#include "bdc.h"

/* Constructor of the non-playable battle unit class (kinds 0x15..0x20): runs
   the CPU-unit base constructor (passing `variant` through), installs the
   class vtable, sets the input's allowed-action mask from the per-kind table
   (index kind - 0x15 clamped to 0..11), fills HP to 200, clears the trailing
   fields and, for kind 0x15, caches model node 1 as the spine node. */
void *BtlUnitAltCtor(void *unit, int kind, s32 variant)
{
    BtlUnitAlt *self = (BtlUnitAlt *)unit;
    BtlBakugan *bakugan = &self->base.base;
    int index;

    BtlCpuUnitCtor(unit, kind, variant);
    bakugan->base.base.vtable = g_btlUnitAltVtbl;
    index = kind - 0x15;
    if (index < 0) {
        index = 0;
    } else if (index > 11) {
        index = 11;
    }
    bakugan->input->allowedActions = g_btlUnitAltActionMasks[index];
    BtlCombatFillHp(&bakugan->combat, 200.0f);
    self->reserved6d0 = 0;
    self->reserved6d4 = 0;
    if (kind == 0x15) {
        bakugan->spineNode = GfxModelGetNode(&bakugan->base, 1);
    }
    self->reserved6d8 = 0;
    return unit;
}
