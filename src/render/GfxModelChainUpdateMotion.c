// bdc 0x089e0f3c GfxModelChainUpdateMotion
#include "bdc.h"

/* Runs virtual method `+0x3c` (the motion update, `GfxModelUpdateAndApplyMotion` in the base
   model vtable `0x08af5484`) on every model of a chain; while the global pause byte `0x08ac5c65`
   (`g_gmoMotionPaused`) is set, models whose `pauseExempt` byte (`+0xba`) is set are skipped. */

void GfxModelChainUpdateMotion(CoreObject *first)

{
  CoreObject *next;

  if (first != (CoreObject *)0x0) {
    if (g_gmoMotionPaused == 0) {
      next = first->next;
      while (1) {
        const VtblEntry *e = &((const VtblEntry *)first->vtable)[7];
        ((void (*)(void *))e->fn)((char *)first + e->delta);
        if (next == (CoreObject *)0x0) break;
        first = next;
        next = next->next;
      }
    } else {
      next = first->next;
      while (1) {
        if (((GfxModel *)first)->pauseExempt == 0) {
          const VtblEntry *e = &((const VtblEntry *)first->vtable)[7];
          ((void (*)(void *))e->fn)((char *)first + e->delta);
        }
        if (next == (CoreObject *)0x0) break;
        first = next;
        next = next->next;
      }
    }
  }
  return;
}
