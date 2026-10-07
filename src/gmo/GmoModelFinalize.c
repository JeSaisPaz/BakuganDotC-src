// bdc 0x08a1d688 GmoModelFinalize
#include "bdc.h"

/* Post-build fix-up of a GMO model data block (0xc0-byte header, see `GmoModelCreate`): builds
   the node draw order `drawNodes` grouped by draw group 0, 1, 2 (`drawGroup` for nodes with parts,
   else 2; group sizes in `drawCount0`/`drawCount1`), links each node to its parent (`parentIndex`:
   flag 1 in the parent's `flags42`, flag 2 only on the parent's last child, `lastChild`), updates
   the texture flags (`GmoModelUpdateTextureFlags`) and sets model flags 0x100/0x200 in `flags02`
   from the class bits of every motion track. */

void GmoModelFinalize(GmoModel *self)

{
  if (self != NULL) {
    s16 *groupStart = self->drawNodes;
    s16 *out = groupStart;
    u32 group;
    s32 i;

    for (group = 0; group < 3; group++) {
      GmoNode *nodes = self->nodes;
      for (i = 0; i < self->nodeCount; i++) {
        u32 kind = 2;
        if (nodes[i].partCount != 0) {
          kind = nodes[i].drawGroup;
        }
        if (kind == group) {
          *out++ = (s16)i;
        }
      }
      if (group == 0) {
        self->drawCount0 = (u16)(out - groupStart);
      } else if (group == 1) {
        self->drawCount1 = (u16)(out - groupStart);
      }
      groupStart = out;
    }

    if (self->nodeCount != 0) {
      GmoNode *nodes = self->nodes;
      for (i = 0; i < self->nodeCount; i++) {
        GmoNode *node = &nodes[i];
        s32 parentIndex = node->parentIndex;
        u16 flags = node->flags42 & 0xfffc;
        node->flags42 = flags;
        node->lastChild = 0;
        if (parentIndex >= 0) {
          GmoNode *parent = &nodes[parentIndex];
          u32 prevChild;
          node->flags42 = flags | 2;
          prevChild = parent->lastChild;
          parent->flags42 |= 1;
          if (prevChild != 0) {
            nodes[prevChild].flags42 &= 0xfffd;
          }
          parent->lastChild = (u16)i;
        }
      }
    }
  }
  GmoModelUpdateTextureFlags(self);
  if (self != NULL) {
    u16 mask = 0;
    u32 motionCount = self->motionCount;
    if (motionCount != 0) {
      GmoMotionInfo *motion = (GmoMotionInfo *)self->motions;
      u32 m;
      for (m = 0; m != motionCount; m++, motion++) {
        u32 trackCount = motion->trackCount;
        GmoMotionTrack *track = motion->tracks;
        u32 t;
        for (t = 0; t < trackCount; t++, track++) {
          if ((track->kind & 0x100) != 0) {
            mask |= 0x100;
          } else {
            mask |= track->kind & 0x200;
          }
        }
      }
    }
    self->flags02 = mask | (self->flags02 & 0xfcff);
  }
}
