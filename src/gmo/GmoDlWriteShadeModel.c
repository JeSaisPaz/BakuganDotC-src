// bdc 0x089dcefc GmoDlWriteShadeModel
#include "bdc.h"

/* Writes the shade model `0x5e`: flat for material type 0x83, smooth otherwise. */

void GmoDlWriteShadeModel(GmoDlContext *self)

{
  u32 *cmd;

  cmd = self->cur;
  self->cur = cmd + 1;
  *cmd = (self->material->type != 0x83) | 0x5e000000;
  return;
}
