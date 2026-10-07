// bdc 0x088fd4ac GameQuestCamModeResetAttached
#include "bdc.h"

/* Calls slot `+0x24` (reset) of the quest camera mode's attached object `+0x90` (vtable at its
   `+4`). */

typedef struct CamAttachedVtbl {
  char pad[0x20];
  VtblEntry reset;
} CamAttachedVtbl;

typedef struct CamAttached {
  void *unk0;
  CamAttachedVtbl *vtbl;
} CamAttached;

void GameQuestCamModeResetAttached(GameQuestCamModeBase *self)

{
  CamAttached *obj = (CamAttached *)self->state;
  const VtblEntry *e = &obj->vtbl->reset;

  ((void (*)(void *))e->fn)((char *)obj + e->delta);
}
