// bdc 0x0881a958 CollisionDeleteColliderList
#include "bdc.h"

/* Deletes every node of the collider list starting at `head` (next pointer `+4`) through its
   virtual destructor (vtable `+0x20`, adjustor `s16` at `+8`, function at `+0xc`, flags 3).
   Does nothing for a NULL head. Called by `CollisionDeleteAllColliders`. */

void CollisionDeleteColliderList(CoreNode *head)
{
  CoreNode *next;

  if (head != (CoreNode *)0) {
    next = head->next;
    while (1) {
      if (head != (CoreNode *)0) {
        const VtblEntry *e = (const VtblEntry *)head->vtable + 1;
        ((void (*)(void *, int))e->fn)((char *)head + e->delta, 3);
      }
      if (next == (CoreNode *)0) break;
      head = next;
      next = next->next;
    }
  }
}
