// bdc 0x088629ac BtlBakuganSelectArtFromCommand
#include "bdc.h"

/* When the unit's virtual predicate (vtable entry 10, `+0x50`) returns non-zero, selects the special
   art from the frame's command bits: 0x80 selects slot 0 (`selectedArtSlot = 0`, combat
   `selectedArt = artIds[0]`), otherwise 0x100 selects slot 1 (`artIds[1]`); with neither bit
   nothing changes. The compiled slot-0 path keeps a dead `slot < 3` bounds check of an inlined
   accessor. */
void BtlBakuganSelectArtFromCommand(BtlBakugan *self)
{
    const VtblEntry *pred = &((const VtblEntry *)self->base.base.vtable)[10];

    if (((s32 (*)(void *))pred->fn)((u8 *)self + pred->delta) == 0) {
        return;
    }
    if ((self->commands & 0x80) != 0) {
        self->selectedArtSlot = 0;
        self->combat.selectedArt = self->combat.artIds[0];
    } else if ((self->commands & 0x100) != 0) {
        self->selectedArtSlot = 1;
        self->combat.selectedArt = self->combat.artIds[1];
    }
}
