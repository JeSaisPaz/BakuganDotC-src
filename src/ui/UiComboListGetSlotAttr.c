// bdc 0x089b2ddc UiComboListGetSlotAttr
#include "bdc.h"

/* Returns the attribute shown in one icon slot of `UiComboList`: takes the
   0x18-byte record array `lists[list]` (terminated by a record whose word `+8` is 0) and returns 2
   (no icon) when it is missing, when `index` is past the end, or when `second` is set but no record
   has a non-zero first byte. Otherwise reads the s16 `+4` (`second`, falling back to `+2` when 0)
   or `+2` of record `index`: a non-zero value gives `value - 1`, zero gives `list & 1`. */

typedef struct ComboSlotRec {
  u8 flag;
  u8 pad1;
  s16 a;
  s16 b;
  u8 pad6[2];
  s32 end;
  u8 padC[12];
} ComboSlotRec;

u32 UiComboListGetSlotAttr(void **lists, s32 index, u32 list, u8 second)
{
  ComboSlotRec *recs = lists[list];
  bool any = false;
  s32 count = 0;
  s32 i;
  s32 v;

  if (recs == NULL) {
    return 2;
  }
  while (recs[count].end != 0) {
    count++;
  }
  for (i = 0; i < count; i++) {
    if (recs[i].flag != 0) {
      any = true;
    }
  }
  if (index >= count) {
    return 2;
  }
  if (second && !any) {
    return 2;
  }
  if (!second) {
    v = recs[index].a;
  } else {
    v = recs[index].b;
    if (v != 0) {
      return v - 1;
    }
    v = recs[index].a;
  }
  if (v == 0) {
    return (list & 1) != 0;
  }
  return v - 1;
}
