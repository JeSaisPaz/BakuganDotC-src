// bdc 0x089dc758 GmoDlModulateNodeColor
#include "bdc.h"

/* Multiplies the model colour (`model+0x40`) with the node colour (`node+0x2c`) into the context's
   colour `ctx+0x5c` (`GmoColorModulate`). */

void GmoDlModulateNodeColor(GmoDlContext *self)

{
  GmoColorModulate(&self->color,&self->model->color,&self->node->color);
  return;
}
