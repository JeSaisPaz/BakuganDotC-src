// bdc 0x0892c21c UiScrollLoopInit
#include "bdc.h"

/* Initialises a 16-byte vertical scroll record with two sprites `a`, `b`: `{on, ..., a, b}`,
   placing `b` 272 px below its current y. */

void UiScrollLoopInit(u8 on, GfxSprite *a, GfxSprite *b, UiScrollLoop *rec)

{
  memset(rec,0,0x10);
  rec->on = on;
  rec->a = a;
  rec->b = b;
  b->posY = b->posY + 272.0f;
  return;
}
