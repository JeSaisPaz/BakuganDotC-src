// bdc 0x088f6a30 GameQuestCamPathModeDtor
#include "bdc.h"

/* Destructor of the path camera mode (vtable g_gameQuestCamPathModeVtbl slot 1): frees the attachment
   records `segment`/`prevSegment` and the path set `pathSet`, then runs the mode base destructor
   (GameQuestCamModeBaseDtor); frees the object when `flags & 1`. */

void GameQuestCamPathModeDtor(GameQuestCamPathMode *self, u32 flags)
{
    void *p;

    if (self == NULL) {
        return;
    }
    p = self->segment;
    self->base.base.base.vtbl = g_gameQuestCamPathModeVtbl;
    if (p != NULL) {
        MemLock();
        MemFree(p, NULL, 0);
        MemUnlock();
        self->segment = NULL;
    }
    p = self->prevSegment;
    if (p != NULL) {
        MemLock();
        MemFree(p, NULL, 0);
        MemUnlock();
        self->prevSegment = NULL;
    }
    p = self->pathSet;
    if (p != NULL) {
        MemLock();
        MemFree(p, NULL, 0);
        MemUnlock();
        self->pathSet = NULL;
    }
    GameQuestCamModeBaseDtor(&self->base, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
