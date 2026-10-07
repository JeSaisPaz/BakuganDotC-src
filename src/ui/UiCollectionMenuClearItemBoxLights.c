// bdc 0x08974310 UiCollectionMenuClearItemBoxLights
#include "bdc.h"

/* Turns every light mesh of the item-box model of `UiCollectionMenu` off
   (vertex colour scale 0 via `GfxModelScaleAmbientColorByName`): `"Card_light"`,
   `"Figure_light"`, `"Sphere_light"`, `"Theater_light"`, `"Mark_light"`, `"Reset_light"`,
   `"button_light"`, `"Rock_light"`. */

void UiCollectionMenuClearItemBoxLights(UiCollectionMenu *self)

{
  GfxModelScaleAmbientColorByName(0.0f,self->itemBox,"Card_light");
  GfxModelScaleAmbientColorByName(0.0f,self->itemBox,"Figure_light");
  GfxModelScaleAmbientColorByName(0.0f,self->itemBox,"Sphere_light");
  GfxModelScaleAmbientColorByName(0.0f,self->itemBox,"Theater_light");
  GfxModelScaleAmbientColorByName(0.0f,self->itemBox,"Mark_light");
  GfxModelScaleAmbientColorByName(0.0f,self->itemBox,"Reset_light");
  GfxModelScaleAmbientColorByName(0.0f,self->itemBox,"button_light");
  GfxModelScaleAmbientColorByName(0.0f,self->itemBox,"Rock_light");
  return;
}

