// bdc 0x0885dd34 BtlBakuganMaterialEnableDepthWrite
#include "bdc.h"

/* Material callback that `BtlBakuganCreateAttachments` runs on every material of the kind-0x11
   model: clears the depth bias and the depth-write-off bit (0x10) of the render flags, so the
   material writes depth again. `arg` is unused. */
void BtlBakuganMaterialEnableDepthWrite(GfxMaterialState *matState, void *arg)
{
    (void)arg;
    matState->depthBias = 0;
    matState->renderFlags &= 0xef;
}
