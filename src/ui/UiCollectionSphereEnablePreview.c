// bdc 0x0897b870 UiCollectionSphereEnablePreview
#include "bdc.h"

/* Resets the preview-model state of `UiCollectionSphere` (`+0x130c`, 0x0c
   bytes) and enables (1) or disables it; read by `UiCollectionSphereSpinPreviewModel`. */

void UiCollectionSphereEnablePreview(UiCollectionSphere *self, u8 enable)

{
  memset(&self->glowOn,0,0xc);
  self->glowOn = enable;
  return;
}

