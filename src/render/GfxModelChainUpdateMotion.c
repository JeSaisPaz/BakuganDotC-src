// bdc 0x089e0f3c GfxModelChainUpdateMotion
#include "bdc.h"

/* Runs virtual method `+0x3c` (the motion update, `GfxModelUpdateAndApplyMotion` in the base
   model vtable `0x08af5484`) on every model of a chain; while the global pause byte `0x08ac5c65`
   (`g_gmoMotionPaused`) is set, models whose byte `+0xba` is set are skipped. */

typedef struct GfxVtEntry {
  short adjust;
  short pad;
  void (*fn)(void *self);
} GfxVtEntry;

/* View of GfxModel's byte `+0xba` (still unnamed in the struct definition). */
typedef struct GfxModelPauseView {
  char pad[0xba];
  u8 pauseExempt;
} GfxModelPauseView;

typedef struct GfxModelVt {
  char slots[0x38];
  GfxVtEntry motionUpdate;
} GfxModelVt;

void GfxModelChainUpdateMotion(CoreObject *first)

{
  CoreObject *next;

  if (first != (CoreObject *)0x0) {
    if (g_gmoMotionPaused == 0) {
      next = first->next;
      while (1) {
        const GfxVtEntry *e = &((const GfxModelVt *)first->vtable)->motionUpdate;
        e->fn((char *)first + e->adjust);
        if (next == (CoreObject *)0x0) break;
        first = next;
        next = next->next;
      }
    } else {
      next = first->next;
      while (1) {
        if (((GfxModelPauseView *)first)->pauseExempt == 0) {
          const GfxVtEntry *e = &((const GfxModelVt *)first->vtable)->motionUpdate;
          e->fn((char *)first + e->adjust);
        }
        if (next == (CoreObject *)0x0) break;
        first = next;
        next = next->next;
      }
    }
  }
  return;
}
