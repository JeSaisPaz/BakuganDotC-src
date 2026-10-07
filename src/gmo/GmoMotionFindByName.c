// bdc 0x089d9d28 GmoMotionFindByName
#include "bdc.h"

/* Returns the registered motion entry whose virtual name-compare method (vtable entry 5) accepts
   `name`, or NULL. `mgr` is unused. */

CoreNode *GmoMotionFindByName(void *mgr, const char *name)
{
  CoreNode *node;
  const VtblEntry *nameCmp;

  for (node = g_gmoMotionRegistry->next; node != NULL; node = node->next) {
    nameCmp = &((const VtblEntry *)node->vtable)[5];
    if (((s32 (*)(void *, const char *))nameCmp->fn)((u8 *)node + nameCmp->delta, name) != 0) {
      return node;
    }
  }
  return NULL;
}
