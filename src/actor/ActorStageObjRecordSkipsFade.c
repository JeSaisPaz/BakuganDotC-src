// bdc 0x088abb28 ActorStageObjRecordSkipsFade
#include "bdc.h"

/* Returns 1 when the object's layout record (`+0x154`) has a kind (`rec+0x32`) of 0x2d
   (`F1_BRIDGE01`), 0x55, 0xbb..0xcc (attribute landmarks) or 0xcf..0xdf, else 0;
   `ActorStageObjUpdate` skips the distance fade for those. */

int ActorStageObjRecordSkipsFade(ActorStageObjBase *self)

{
  short kind;

  if (self->record == (void *)0x0) {
    return 0;
  }
  kind = ((ActorStageObjRecord *)self->record)->field32[0];
  if (kind < 0xbb) {
    if (kind < 0x2e) {
      if (kind < 0x2d) {
        return 0;
      }
    }
    else if (kind != 0x55) {
      return 0;
    }
  }
  else if (kind < 0xcf) {
    if (0xcc < kind) {
      return 0;
    }
  }
  else if (0xdf < kind) {
    return 0;
  }
  return 1;
}
