// bdc 0x089d9c54 GmoMotionIndexOfName
#include "bdc.h"

/* Returns the index of the motion called `name` in the registry, loading `name + ".bin"` with
   `GmoMotionLoadFile` when it is not registered yet (the new entry's index is then the old
   count). Returns -1 when the load failed. */

s32 GmoMotionIndexOfName(void *mgr, const char *name)
{
  CoreNode *node;
  const VtblEntry *nameCmp;
  s32 index = 0;
  char path[76];

  for (node = g_gmoMotionRegistry->next; node != NULL; node = node->next) {
    nameCmp = &((const VtblEntry *)node->vtable)[5];
    if (((s32 (*)(void *, const char *))nameCmp->fn)((u8 *)node + nameCmp->delta, name) != 0) {
      return index;
    }
    index++;
  }
  strcpy(path, name);
  strcat(path, ".bin");
  if (GmoMotionLoadFile(mgr, path) != NULL) {
    return index;
  }
  return -1;
}
