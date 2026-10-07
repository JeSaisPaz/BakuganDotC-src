// bdc 0x088d8ce4 GameFieldPointListDestroy
#include "bdc.h"

/* Destroys the field point list `0x08abf0a8`: deletes every 0x40-byte point object through its
   virtual destructor (slot 1, flags 3), frees the list head and clears the global. Called from
   `GameFieldDtor`. */

void GameFieldPointListDestroy(void)
{
  GameFieldPoint *p;
  GameFieldPoint *next;

  if (g_gameFieldPointList != NULL) {
    p = *g_gameFieldPointList;
    if (p != NULL) {
      next = p->next;
      while (1) {
        if (p != NULL) {
          const VtblEntry *dtor = &((const VtblEntry *)p->vtable)[1];
          ((void (*)(void *, s32))dtor->fn)((u8 *)p + dtor->delta, 3);
        }
        if (next == NULL) break;
        p = next;
        next = next->next;
      }
    }
    if (g_gameFieldPointList != NULL) {
      MemLock();
      MemFree(g_gameFieldPointList, NULL, 0);
      MemUnlock();
      g_gameFieldPointList = NULL;
    }
  }
}
