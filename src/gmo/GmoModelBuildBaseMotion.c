// bdc 0x08a1d888 GmoModelBuildBaseMotion
#include "bdc.h"

/* Rebuilds every motion slot of a model (motion count `+0x20` plus the extra base slot
   `GmoModelBuild` reserves) over one shared channel list, so that each slot holds one track per
   channel that any motion animates. Channels are the distinct (attribute, target) pairs of all
   tracks (up to 0x200; attributes 0x49..0x4b merged into 0x4b) that at least one track without
   kind bit 2 uses. Returns 0 when the model has no motions or no nodes and materials, when the
   channel list overflows, when every slot already holds every channel, or when the plan commit
   fails. Otherwise it reserves the new track arrays and one key block per channel (sized by
   attribute: 0x42/0x4f/0x86 4 bytes, 0x43 0x20, 0x47 0x40, 0x48/0x4c/0x4d/0xe1 0xc,
   0x49..0x4b/0x82/0x84/0x85/0x98 0x10, 0x83 0x14) in a private plan, applies the old base slot
   at frame 0 (`GmoMotionApplyTrack`), commits the plan and moves each slot's tracks
   (`GmoTrackCopy`) into the new arrays, releasing the old ones. Each channel missing in a slot
   gets a single-key track holding the current node or material value (material getters
   `GmoMaterialGetDiffuse`, `GmoMaterialGetSpecularColor`, …), or a copy of the track the
   previous slot got. Then it frees the plan and returns 1. */

s32 GmoModelBuildBaseMotion(GmoModel *self)

{
  s32 plan[0x6c / 4];
  u32 channels[0x200]; /* target | attribute << 16 | use count << 24 */
  float values[16] __attribute__((aligned(16)));
  u32 rgba;
  GmoMotionInfo *info;
  GmoMotionTrack *track;
  GmoMotionTrack *src;
  GmoMotionTrack *newTracks;
  u16 *newCursors;
  u32 *out;
  u32 *block;
  const float *from;
  GmoNode *node;
  GmoMaterial *mat;
  u32 key;
  u32 attr;
  u32 target;
  u16 bits;
  int slots;
  int total;
  int n;
  int m;
  int i;
  int idx;
  int c;
  int size;
  int count;

  if (self == NULL || self->motionCount == 0) {
    return 0;
  }
  if (self->nodeCount == 0 && self->materialCount == 0) {
    return 0;
  }
  GmoPlanInit(plan);

  /* Collect the distinct channels, counting the tracks without kind bit 2 per channel. */
  total = 0;
  slots = self->motionCount + 1;
  n = 0;
  for (m = 0; m < slots; m++) {
    info = (GmoMotionInfo *)self->motions + m;
    for (i = 0; i < info->trackCount; i++) {
      track = &((GmoMotionTrack *)PspPtr(info->tracks))[i];
      attr = track->paramD;
      if (attr - 0x49 < 2) {
        attr = 0x4b;
      }
      key = track->ref | (attr & 0xff) << 16;
      idx = -1;
      if (i < n && (channels[i] & 0xffffff) == key) {
        idx = i;
      }
      else {
        for (c = 0; c < n; c++) {
          if ((channels[c] & 0xffffff) == key) {
            idx = c;
            break;
          }
        }
      }
      if (idx < 0) {
        if (n < 0x200) {
          channels[n] = key;
          idx = n;
          n++;
        }
        if (idx < 0) {
          return 0;
        }
      }
      if ((track->kind & 2) == 0) {
        channels[idx] += 1u << 24; /* use count byte */
      }
    }
    total += info->trackCount;
  }
  if (n * slots == total) {
    return 0;
  }

  /* Keep only the channels some track uses. */
  out = channels;
  for (i = 0; i < n; i++) {
    if ((channels[i] >> 24) != 0) {
      *out++ = channels[i];
    }
  }
  n = (int)(out - channels);

  /* Reserve the new track and cursor arrays of every slot and one key block per channel. */
  if (slots > 0) {
    for (m = 0; m < self->motionCount + 1; m++) {
      GmoPlanReserveRec10A(n, plan);
      GmoPlanReserve(plan, 0, 4, n * 2);
    }
  }
  for (i = 0; i < n; i++) {
    switch ((channels[i] >> 16) & 0xff) {
    case 0x42:
    case 0x4f:
    case 0x86:
      size = 4;
      break;
    case 0x43:
      size = 0x20;
      break;
    case 0x47:
      size = 0x40;
      break;
    case 0x48:
    case 0x4c:
    case 0x4d:
    case 0xe1:
      size = 0xc;
      break;
    case 0x49:
    case 0x4a:
    case 0x4b:
    case 0x82:
    case 0x84:
    case 0x85:
    case 0x98:
      size = 0x10;
      break;
    case 0x83:
      size = 0x14;
      break;
    default:
      size = 0;
      break;
    }
    GmoPlanReserve(plan, 0, 4, size + 4);
  }

  /* Apply the old base slot at frame 0. */
  info = (GmoMotionInfo *)self->motions + self->motionCount;
  for (i = 0; i < info->trackCount; i++) {
    GmoMotionApplyTrack(0.0f, g_gmoBaseMotionWeight, (GmoMotionTrack *)PspPtr(info->tracks) + i,
                        (u16 *)PspPtr(info->table) + i, self);
  }
  if (GmoPlanCommit(plan) == 0) {
    return 0;
  }

  /* Move every slot's tracks to their channel index in a new array. */
  for (m = 0; m < self->motionCount + 1; m++) {
    info = (GmoMotionInfo *)self->motions + m;
    newTracks = GmoPlanTakeRec10A(n, plan);
    newCursors = GmoPlanTake(plan, 0, 4, n * 2);
    memset(newCursors, 0, n * 2);
    for (i = 0; i < info->trackCount; i++) {
      track = &((GmoMotionTrack *)PspPtr(info->tracks))[i];
      if ((track->kind & 2) != 0) {
        continue;
      }
      attr = track->paramD;
      if (attr - 0x49 < 2) {
        attr = 0x4b;
      }
      key = track->ref | (attr & 0xff) << 16;
      idx = -1;
      if (i < n && (channels[i] & 0xffffff) == key) {
        idx = i;
      }
      else {
        for (c = 0; c < n; c++) {
          if ((channels[c] & 0xffffff) == key) {
            idx = c;
            break;
          }
        }
      }
      if (idx >= 0) {
        GmoTrackCopy(&newTracks[idx], track, 0, plan);
      }
    }
    GmoMotionTrackArrayRelease((short *)PspPtr(info->tracks), info->trackCount);
    GmoHeapReleaseThunk(0, PspPtr(info->table));
    info->tracks = PspAddr(newTracks);
    info->trackCount = (u16)n;
    info->table = PspAddr(newCursors);
  }

  /* Fill the channels a slot lacks: the first such slot gets the current value, later ones a
     copy of the previous filled track. */
  for (c = 0; c < n; c++) {
    src = NULL;
    for (m = 0; m < self->motionCount + 1; m++) {
      track = &((GmoMotionTrack *)PspPtr(((GmoMotionInfo *)self->motions)[m].tracks))[c];
      if (track->data != 0) {
        continue;
      }
      if (src != NULL) {
        GmoTrackCopy(track, src, 0, plan);
        src = track;
        continue;
      }
      src = track;
      key = channels[c];
      attr = (key >> 16) & 0xff;
      target = key & 0xffff;
      count = 0;
      if (attr - 0x82 < 0x17) {
        /* Material channel (GmoModelGetMaterial inlined). */
        if (((target + 1) & 0xffff0000) != 0) {
          mat = (GmoMaterial *)(uintptr_t)target;
        }
        else {
          if (target >= self->materialCount) {
            continue;
          }
          mat = (GmoMaterial *)self->materials + target;
        }
        if (mat == NULL) {
          continue;
        }
        switch (attr) {
        case 0x82:
          rgba = GmoMaterialGetDiffuse(mat);
          count = 4;
          GmoColorFromRgba8(values, &rgba);
          break;
        case 0x83:
          rgba = GmoMaterialGetSpecularColor(mat);
          values[4] = GmoMaterialGetSpecularPower(mat);
          count = 5;
          GmoColorFromRgba8(values, &rgba);
          break;
        case 0x84:
          rgba = GmoMaterialGetEmission(mat);
          count = 4;
          GmoColorFromRgba8(values, &rgba);
          break;
        case 0x85:
          rgba = GmoMaterialGetSpecularColor(mat);
          count = 4;
          GmoColorFromRgba8(values, &rgba);
          break;
        case 0x86:
          values[0] = GmoMaterialGetReflection(mat);
          count = 1;
          break;
        case 0x87:
        case 0x88:
          count = 1;
          values[0] = 0.0f;
          break;
        case 0x98:
          from = GmoMaterialGetAttrData(mat, 0);
          if (from == NULL) {
            from = g_gmoBaseMotionAttrData;
          }
          count = 4;
          values[0] = from[0];
          values[1] = from[1];
          values[2] = from[2];
          values[3] = from[3];
          break;
        default:
          continue;
        }
      }
      else {
        /* Node channel (node lookup inlined like GmoModelGetMaterial). */
        if (((target + 1) & 0xffff0000) != 0) {
          node = (GmoNode *)(uintptr_t)target;
        }
        else {
          if (target >= self->nodeCount) {
            continue;
          }
          node = &self->nodes[target];
        }
        if (node == NULL) {
          continue;
        }
        if (attr < 0x4c) {
          if (attr >= 0x49) {
            from = node->rotate;
            count = 4;
          }
          else if (attr == 0x43) {
            from = node->morphWeights;
            count = node->morphCount;
            if (from == NULL) {
              from = g_gmoBaseMotionMorphWeights;
              count = 8;
            }
          }
          else if (attr < 0x44) {
            if (attr != 0x42) {
              continue;
            }
            from = NULL;
            values[0] = (float)(u32)node->visible;
            count = 1;
          }
          else if (attr == 0x47) {
            from = node->matrix;
            count = 0x10;
            if (from == NULL) {
              from = (const float *)&g_gmoIdentityMatrix;
            }
          }
          else if (attr == 0x48) {
            from = node->translate;
            count = 3;
          }
          else {
            continue;
          }
        }
        else if (attr == 0x4f) {
          from = NULL;
          values[0] = node->morphPos;
          count = 1;
        }
        else if (attr == 0x4c || attr == 0x4d || attr == 0xe1) {
          from = node->scale;
          count = 3;
        }
        else {
          continue;
        }
        if (from != NULL && count > 0) {
          memcpy(values, from, count * 4);
        }
        if (count == 0) {
          continue;
        }
      }

      block = GmoPlanTake(plan, 0, 4, count * 4 + 1);
      memcpy(block + 1, values, count * 4);
      block[0] = 0;
      attr = (key >> 16) & 0xff;
      if (attr - 0x48 < 6 || attr == 0xe1) {
        bits = 0x101;
      }
      else if (attr - 0x42 < 6 || attr == 0x4f) {
        bits = 0x100;
      }
      else if (attr - 0x82 < 7 || attr == 0x98) {
        bits = 0x200;
      }
      else {
        bits = 0;
      }
      track->kind = track->kind | bits | 2;
      track->paramD = (u8)attr;
      track->paramC = (u8)count;
      track->paramA = 1;
      track->param8 = 0;
      track->ref = (u16)key;
      track->data = PspAddr(block);
    }
  }
  GmoPlanFree(plan);
  return 1;
}
