// bdc 0x088be234 GameFieldFindTask
#include "bdc.h"

/* Returns the field task (id 500, `GameFieldCtor`) or NULL (`CoreTaskFind`). Widely used to
   reach the field's object lists (`+0x658` gimmicks, `+0x664` stage objects). */

CoreTask *GameFieldFindTask(void)
{
  return CoreTaskFind(500);
}
