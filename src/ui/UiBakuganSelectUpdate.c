// bdc 0x0892b784 UiBakuganSelectUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiBakuganSelect screen (task id 371): runs the current
   phase handler from the 4-entry pointer-to-member phase table `g_uiBakuganSelectPhaseTable` (indexed by
   `phase`, +0x28), then `UiBakuganSelectUpdateModel`, then `UiScreenUpdateCommon` and, unless a close
   was requested, `UiScreenUpdateBg`. */

typedef struct BakuganPhaseEntry {
  s16 thisAdjust; /* +0 */
  s16 vtIndex;    /* +2: nonzero = virtual, index into the vtable */
  void *fn;       /* +4: function, or vtable offset when virtual */
} BakuganPhaseEntry;

typedef struct BakuganVtblEntry {
  s16 thisAdjust;
  s16 pad;
  void (*fn)(void *);
} BakuganVtblEntry;

void UiBakuganSelectUpdate(UiBakuganSelect *self)

{
  u32 phase = self->base.phase;
  if ((s32)phase >= 0 && phase < 4) {
    const BakuganPhaseEntry *e = (const BakuganPhaseEntry *)g_uiBakuganSelectPhaseTable + phase;
    u8 *obj = (u8 *)self + e->thisAdjust;
    void (*fn)(void *) = (void (*)(void *))e->fn;
    if (e->vtIndex != 0) {
      const BakuganVtblEntry *v = (const BakuganVtblEntry *)(*(u8 **)(obj + (intptr_t)e->fn)) + e->vtIndex;
      fn = v->fn;
      obj += v->thisAdjust;
    }
    fn(obj);
  }
  UiBakuganSelectUpdateModel(self);
  u8 closing = self->base.closeRequested;
  UiScreenUpdateCommon(&self->base);
  if (closing == 0) {
    UiScreenUpdateBg(&self->base);
  }
}
