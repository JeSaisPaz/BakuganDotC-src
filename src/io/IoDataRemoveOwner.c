// bdc 0x089fbe54 IoDataRemoveOwner
#include "bdc.h"

/* Removes the reference of `owner` (any reference when `owner` is 0) from the owner list of a data
   request (`COData`) and deletes the node (pool or heap). */

void IoDataRemoveOwner(IoData *self, void *owner)

{
  CoreNode *head;
  CoreNode *node;
  const VtblEntry *vtbl;

  head = self->owners;
  if (head != (CoreNode *)0x0) {
    node = head;
    while (((IoDataOwnerRef *)node)->owner != owner && owner != (void *)0x0) {
      node = node->next;
      if (node == (CoreNode *)0x0) {
        return;
      }
    }
    if (head == node) {
      self->owners = node->next;
    }
    if (g_ioDataRefPool != (MemPool *)0x0 && MemPoolFree(g_ioDataRefPool, node)) {
      vtbl = (const VtblEntry *)node->vtable;
      ((void (*)(void *, int))vtbl[1].fn)((u8 *)node + vtbl[1].delta, 2);
      node = (CoreNode *)0x0;
    }
    if (node != (CoreNode *)0x0) {
      vtbl = (const VtblEntry *)node->vtable;
      ((void (*)(void *, int))vtbl[1].fn)((u8 *)node + vtbl[1].delta, 3);
    }
  }
}
