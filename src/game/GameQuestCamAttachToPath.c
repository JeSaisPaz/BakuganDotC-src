// bdc 0x088f7278 GameQuestCamAttachToPath
#include "bdc.h"

/* Re-attaches a path camera mode to the nearest path segment. Nothing happens when the mode's path
   node `node` is -1. Otherwise it saves the current attachment `segment` (or a zeroed record),
   looks up the nearest segment in the current camera set of `g_questCamTable`
   (`GameQuestPathFindNearestSegmentRetry`) and, when one was found, refreshes `prevSegment`: a
   copy of `segment` (allocated 0x30 bytes from the low heap if missing) when the current set is
   the table's first set, else the nearest segment in the first set. If the attached segment's
   nodes are unchanged, `retries` is reset. Otherwise `retries` is incremented: below 5 the old
   attachment is restored (the change is damped); at 5 the change is accepted, the old direction
   becomes `defaultDir`, the path-set factory `pathSet` is notified through its vtable entry 1
   with `&segment`, and `retries` is reset. A newly allocated `prevSegment` gets a zero `dir`
   (VFPU bank constant C720). */

void GameQuestCamAttachToPath(GameQuestCamPathMode *self)
{
  GameQuestPathSegment saved;
  GameQuestPathSegment *seg;
  GameQuestPathSegment *prev;
  GameQuestPathSegment *alloc;
  GameQuestCamPtrVec *curSet;
  GameQuestCamPtrVec *firstSet;
  GameQuestCamTable *table;
  const VtblEntry *vtbl;
  bool fromLow;

  if (self->base.node == -1) {
    return;
  }
  /* the asm first stores bank constant C720 (zero) to saved.dir; overwritten here */
  saved.from = NULL;
  saved.to = NULL;
  saved.dir = g_gameQuestCamPathModeAxisConsts.zero;
  saved.t = 0.0f;
  saved.dist = 0.0f;
  if (self->segment != NULL) {
    seg = (GameQuestPathSegment *)self->segment;
    saved.from = seg->from;
    saved.to = seg->to;
    saved.dir = seg->dir;
    saved.t = seg->t;
    saved.dist = seg->dist;
  }
  curSet = g_questCamTable->cur;
  GameQuestPathFindNearestSegmentRetry(self, (void *)&self->segment, (void *)curSet, self->base.node);
  if (self->segment == NULL) {
    return;
  }

  /* inlined bounds-checked table->data[0] */
  table = g_questCamTable;
  if (0 < table->count) {
    firstSet = (GameQuestCamPtrVec *)table->data[0];
  } else {
    memset(&g_gameQuestCamNullSet, 0, 4);
    firstSet = g_gameQuestCamNullSet;
  }

  if (curSet != firstSet) {
    GameQuestPathFindNearestSegmentRetry(self, (void *)&self->prevSegment, (void *)firstSet,
                                         self->base.node);
  } else if (self->segment != NULL) {
    if (self->prevSegment == NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      alloc = MemAlloc(sizeof(GameQuestPathSegment), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (alloc != NULL) {
        alloc->dir.x = 0.0f;
        alloc->dir.y = 0.0f;
        alloc->dir.z = 0.0f;
        alloc->dir.w = 0.0f;
      }
      self->prevSegment = alloc;
    }
    ((GameQuestPathSegment *)self->prevSegment)->from = ((GameQuestPathSegment *)self->segment)->from;
    ((GameQuestPathSegment *)self->prevSegment)->to = ((GameQuestPathSegment *)self->segment)->to;
    ((GameQuestPathSegment *)self->prevSegment)->t = ((GameQuestPathSegment *)self->segment)->t;
    prev = (GameQuestPathSegment *)self->prevSegment;
    prev->dir = ((GameQuestPathSegment *)self->segment)->dir;
    ((GameQuestPathSegment *)self->prevSegment)->dist = ((GameQuestPathSegment *)self->segment)->dist;
  }

  if (saved.from == ((GameQuestPathSegment *)self->segment)->from &&
      saved.to == ((GameQuestPathSegment *)self->segment)->to) {
    self->retries = 0;
    return;
  }
  self->retries = self->retries + 1;
  if (self->retries < 5) {
    seg = (GameQuestPathSegment *)self->segment;
    seg->from = saved.from;
    seg->to = saved.to;
    seg->dir = saved.dir;
    seg->t = saved.t;
    seg->dist = saved.dist;
    return;
  }
  self->defaultDir = saved.dir;
  if (self->pathSet != NULL) {
    vtbl = *(const VtblEntry **)self->pathSet;
    ((void (*)(void *, void **))vtbl[1].fn)((char *)self->pathSet + vtbl[1].delta, &self->segment);
  }
  self->retries = 0;
}
