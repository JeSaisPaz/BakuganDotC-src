// bdc 0x088d8ce4 GameFieldPointListDestroy
#include "bdc.h"

/* Destroys the field point list `0x08abf0a8`: deletes every 0x40-byte point object through its
   virtual destructor (slot 1, flags 3), frees the list head and clears the global. Called from
   `GameFieldDtor`. */

typedef struct GameFieldPointDtorSlot {
  s16 thisAdjust;
  s16 pad;
  void (*fn)(void *self, s32 flags);
} GameFieldPointDtorSlot;

/* vtable slot 1 (`vtable + 8`): this-adjust and destructor. */

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
          const GameFieldPointDtorSlot *slot = (const GameFieldPointDtorSlot *)p->vtable + 1;
          slot->fn((u8 *)p + slot->thisAdjust, 3);
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
