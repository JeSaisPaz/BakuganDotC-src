// bdc 0x0885dd10 BtlBakuganHadesMaterialCallback
#include "bdc.h"

/* Material callback that `BtlBakuganCreateAttachments` runs on every material of the kind-0x1a
   (Hades) model after binding the `10_D_Hades_Refrec` texture to slot 6: sets bits 5..7 of the
   shade flags to 7 and bits 0..1 of the render flags to 2. */
void BtlBakuganHadesMaterialCallback(void *matState, void *arg)
{
    GfxMaterialState *state = matState;

    (void)arg;
    state->shadeFlags |= 0xe0;
    state->renderFlags = (state->renderFlags & 0xfc) | 2;
}
