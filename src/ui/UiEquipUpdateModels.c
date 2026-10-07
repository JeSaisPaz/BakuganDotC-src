// bdc 0x0895787c UiEquipUpdateModels
#include "bdc.h"

/* Runs the virtual update (vtable entry 7) of each of the four Bakugan models `bakuganModels` of
   `UiEquip` that exists, in order 0..3. */
void UiEquipUpdateModels(UiEquip *self)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        GfxModel *model = self->bakuganModels[i];

        if (model != NULL) {
            const VtblEntry *update = &((const VtblEntry *)model->base.vtable)[7];

            ((void (*)(void *))update->fn)((u8 *)model + update->delta);
        }
    }
}
