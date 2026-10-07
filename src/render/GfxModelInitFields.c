// bdc 0x089de5f4 GfxModelInitFields
#include "bdc.h"

/* Initialises the fields of a GMO model object: `pos` and `basisY` = (0,0,0,1) (bank C730),
   `rot` and `velocity` = (0,0,0,0) (bank C720), scale (1,1,1,0), `ambient` and `color` copied
   from `g_colorWhite`, motion speed 1, visible 1, fog/lighting/flags off, name `"No Name"`,
   a new GMO data block (`GmoModelCreate`) whose root matrix is set to identity, motion slot 0,
   registry index -1, motion off, no gmo, no sound. */

void GfxModelInitFields(GfxModel *self)

{
  GmoModel *data;
  int i;

  self->pos[0] = 0.0f;
  self->pos[1] = 0.0f;
  self->pos[2] = 0.0f;
  self->pos[3] = 1.0f;
  for (i = 0; i < 4; i++) {
    self->rot[i] = 0.0f;
  }
  for (i = 0; i < 4; i++) {
    self->velocity[i] = 0.0f;
  }
  self->scale[0] = 1.0f;
  self->scale[1] = 1.0f;
  self->scale[2] = 1.0f;
  self->scale[3] = 0.0f;
  self->basisY[0] = 0.0f;
  self->basisY[1] = 0.0f;
  self->basisY[2] = 0.0f;
  self->basisY[3] = 1.0f;
  self->ambient[0] = g_colorWhite.x;
  self->ambient[1] = g_colorWhite.y;
  self->ambient[2] = g_colorWhite.z;
  self->ambient[3] = g_colorWhite.w;
  self->color[0] = g_colorWhite.x;
  self->color[1] = g_colorWhite.y;
  self->color[2] = g_colorWhite.z;
  self->color[3] = g_colorWhite.w;
  self->motionSpeed = 1.0f;
  self->motionEnded = 0;
  self->visible = 1;
  self->fogEnabled = 0;
  self->lighting = 0;
  self->reservedBa = 0;
  strcpy(self->name, "No Name");
  self->materialStates = NULL;
  data = GmoModelCreate();
  self->data = data;
  for (i = 0; i < 16; i++) {
    data->rootMatrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
  }
  self->motionSlot = 0;
  self->motionIndex = -1;
  self->registryMotions = 0;
  self->ownsGmo = 0;
  self->motionEnabled = 0;
  self->gmo = NULL;
  self->sound = NULL;
}
