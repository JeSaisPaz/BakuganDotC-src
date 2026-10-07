// bdc 0x089a6bd4 UiMainMenuDimItemBoxLights
#include "bdc.h"

/* Turns off the item box's four category lights (`Card_light`, `Figure_light`, `Sphere_light`,
   `Theater_light`) on the base model `+0x69c` (vertex-colour scale 0). */

void UiMainMenuDimItemBoxLights(UiMainMenu *self)

{
  GfxModelScaleAmbientColorByName(0.0f,self->models[4],"Card_light");
  GfxModelScaleAmbientColorByName(0.0f,self->models[4],"Figure_light");
  GfxModelScaleAmbientColorByName(0.0f,self->models[4],"Sphere_light");
  GfxModelScaleAmbientColorByName(0.0f,self->models[4],"Theater_light");
  return;
}

