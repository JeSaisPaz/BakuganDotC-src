// bdc 0x088f8d08 GameQuestCamSpringGetOwnerPos
#include "bdc.h"

/* Returns the position of the spring's owner (`+8`) through the owner's virtual slot 3 (`+0x1c`). */

typedef struct CamOwner {
  void *unk0;
  const VtblEntry *vtbl; /* +0x04 */
} CamOwner;

float *GameQuestCamSpringGetOwnerPos(GameQuestCamSpring *self)

{
  CamOwner *owner = (CamOwner *)self->followed;
  const VtblEntry *e = &owner->vtbl[3];

  return ((float *(*)(void *))e->fn)((char *)owner + e->delta);
}
