// bdc 0x088cc1d8 UiTalkBalloonCreate
#include "bdc.h"

/* Allocates (low heap, 0x220 bytes) and constructs a talk balloon (`UiTalkBalloonCtor`), creates
   its printer for `font` (`UiTalkBalloonCreatePrinter`), copies `pos` (default
   `g_uiTalkBalloonVecB` when NULL) to the printer layer origin `view.w` and to `homePos`, sets
   the text (`UiTalkBalloonSetText`), lays it out (`UiTalkBalloonLayout`), sets task id 420
   and inserts the task (`CoreTaskInsert`, priority 0). Returns the task (NULL if the
   allocation failed; the later calls are then made on NULL anyway). */

UiTalkBalloon *UiTalkBalloonCreate(void *owner, float *pos, char **font, const char **style)

{
  bool fromLow;
  UiTalkBalloon *mem;
  UiTalkBalloon *self;
  UiTalkBalloon *result;
  const float *src;

  result = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x220, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem == NULL) {
    self = result;
  }
  else {
    UiTalkBalloonCtor(mem);
    result = mem;
    self = result;
  }
  UiTalkBalloonCreatePrinter(self, font);
  if (pos != NULL) {
    src = pos;
  }
  else {
    src = &g_uiTalkBalloonVecB.x;
  }
  self->printer->layer.view.w.x = src[0];
  self->printer->layer.view.w.y = src[1];
  self->printer->layer.view.w.z = src[2];
  self->printer->layer.view.w.w = src[3];
  self->homePos[0] = src[0];
  self->homePos[1] = src[1];
  self->homePos[2] = src[2];
  self->homePos[3] = src[3];
  UiTalkBalloonSetText(self, owner);
  UiTalkBalloonLayout(self, font, style);
  self->base.id = 0x1a4;
  CoreTaskInsert(self, 0);
  return result;
}
