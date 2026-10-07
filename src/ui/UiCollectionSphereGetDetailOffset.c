// bdc 0x0897d588 UiCollectionSphereGetDetailOffset
#include "bdc.h"

/* Writes the detail-view position offset (x, y) of the selected entry of
   `UiCollectionSphere` into `out`. Categories 0/1 use
   `g_uiCollectionSphereDetailOffsets` by the id of cell `cursor + page * 6`; other categories
   ask `UiCollectionSphereGetPageKind`: kind 0 or 0xff (model page) uses
   `g_uiCollectionSphereDetailOffsetsAlt` by the id of cell `cursor + (page / 3) * 6`, kinds
   1 and 2 use `g_uiCollectionSphereDetailOffsets` by `kind1Ids`/`kind2Ids[page / 3]`. Any other
   kind writes an uninitialised stack pair (see Notes). */

void UiCollectionSphereGetDetailOffset(float *out, UiScreen *screen)
{
  UiCollectionSphere *self = (UiCollectionSphere *)screen;
  float offsets[33][2];
  float offsetsAlt[13][2];
  float off[2];
  u8 kind;

  memcpy(offsets, g_uiCollectionSphereDetailOffsets, sizeof(offsets));
  memcpy(offsetsAlt, g_uiCollectionSphereDetailOffsetsAlt, sizeof(offsetsAlt));
  if (self->category >= 0 && self->category < 2) {
    u8 id = self->entryIds[self->cursor + self->page * 6];
    off[0] = offsets[id][0];
    off[1] = offsets[id][1];
  }
  else {
    kind = UiCollectionSphereGetPageKind(self, (u8)self->page);
    if (kind == 0xff || kind == 0) {
      u8 id = self->entryIds[self->cursor + (self->page / 3) * 6];
      off[0] = offsetsAlt[id][0];
      off[1] = offsetsAlt[id][1];
    }
    else if (kind < 2) {
      u8 id = self->kind1Ids[self->page / 3];
      off[0] = offsets[id][0];
      off[1] = offsets[id][1];
    }
    else if (kind < 3) {
      u8 id = self->kind2Ids[self->page / 3];
      off[0] = offsets[id][0];
      off[1] = offsets[id][1];
    }
    /* UB (original binary): kind >= 3 leaves off (stack 0x0/0x4) unset. */
  }
  out[0] = off[0];
  out[1] = off[1];
}
