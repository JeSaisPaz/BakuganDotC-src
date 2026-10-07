// bdc 0x0889b4f4 BtlAiReleaseKindParams
#include "bdc.h"

/* Destroys the three per-species parameter objects `params[0..2]` of a CPU AI (built by
   `BtlAiCreateKindParamSet`: a `BtlAiParams` then two `BtlAiParamSet`s, all `{kind, vtbl}`)
   in order through vtable entry 1 (deleting destructor, flags 3) and clears each non-NULL pointer.
   Does nothing when `params` is NULL; `cache` is unused. */
void BtlAiReleaseKindParams(void *cache, void **params)
{
  int i;

  (void)cache;
  if (params == NULL) {
    return;
  }
  for (i = 0; i < 3; i++) {
    BtlAiParamSet *obj = (BtlAiParamSet *)params[i];

    if (obj != NULL) {
      const VtblEntry *dtor = &obj->vtbl[1];

      ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
      params[i] = NULL;
    }
  }
}
