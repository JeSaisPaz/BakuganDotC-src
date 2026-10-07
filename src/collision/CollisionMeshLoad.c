// bdc 0x089e61a0 CollisionMeshLoad
#include "bdc.h"

/* Builds the triangle-mesh collision shape (type 8) of a shape block from its collision file
   (`desc`, a CollisionFile): counts the `MESH` chunks into `partCount` (`CollisionCountMeshChunks`)
   and allocates the `parts` pointer array from the low heap (and, with `keepWorldVerts`, 0xc0 bytes
   per part at `worldVerts` that receive writable copies of the MESH part headers). `surfaceIds` is
   filled with -1, then the chunk list is walked: each of the first 16 `ATTR` chunks maps its name to
   a surface id by `strcasecmp` against `g_collisionSurfaceNames` (ids 15/17 become 4, 16/18 become
   6, no match leaves -1); `MESH` starts a new part (copied when `keepWorldVerts`); `VERT`, `NORM`,
   `FACE` and `GEOM` are linked into the current part; `AABB` gives its BVH root/triangle data.
   Unless the file's `flags` bit 15 is already set, `AABB` offsets are relocated in place
   (`CollisionBvhRelocate`) and every `GEOM` (translation, rotation angles, scale rows) is baked
   into a 4x4 matrix in place (scale times the X rotation only); the bit is set at the end. */

void CollisionMeshLoad(void *block, bool keepWorldVerts)

{
  CollisionShapeBlock *self = (CollisionShapeBlock *)block;
  CollisionFile *file;
  CollisionFileChunk *chunk;
  CollisionFacePart *part;
  CollisionBvhPart *aabb;
  u8 *data;
  void *mem;
  bool fromLow;
  s32 count;
  s32 partIdx;
  s32 attrIdx;
  s32 worldOffset;
  s32 i;
  u16 chunkIdx;
  ScePspFMatrix4 tmp;
  ScePspFMatrix4 *geom;
  float sx;
  float sy;
  float sz;
  float rc;
  float rs;

  file = (CollisionFile *)self->desc;
  chunkIdx = 0;
  chunk = (CollisionFileChunk *)file->chunks;
  partIdx = 0;
  part = (CollisionFacePart *)0;
  attrIdx = 0;
  count = CollisionCountMeshChunks(chunk, file->chunkCount);
  self->partCount = count;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(count << 2, (const char *)0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->parts = (void **)mem;
  if (keepWorldVerts) {
    count = self->partCount;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(count * 0xc0, (const char *)0, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->worldVerts = mem;
  }
  self->surfaceIdWords[3] = -1;
  self->surfaceIdWords[2] = -1;
  self->surfaceIdWords[1] = -1;
  self->surfaceIdWords[0] = -1;
  if (chunkIdx < file->chunkCount) {
    worldOffset = 0;
    do {
      data = chunk->data + (chunk->sizeAndPad >> 24);
      if (chunk->tag == g_attrChunkTag) {
        if (attrIdx < 16) {
          for (i = 0; i < 19; i++) {
            if (strcasecmp(g_collisionSurfaceNames[i], (const char *)data) == 0) {
              if (i < 15) {
                self->surfaceIds[attrIdx] = (s8)i;
              }
              else if (i == 15 || i == 17) {
                /* Damage_Ground, Quicksand_Ground -> Water_ground */
                self->surfaceIds[attrIdx] = 4;
              }
              else {
                /* Damage_Zone, Quicksand_Zone -> Water */
                self->surfaceIds[attrIdx] = 6;
              }
              break;
            }
          }
          attrIdx++;
        }
      }
      else if (chunk->tag == g_meshChunkTag) {
        part = (CollisionFacePart *)data;
        if (keepWorldVerts) {
          memcpy((u8 *)self->worldVerts + worldOffset, part, 0xc0);
          part = (CollisionFacePart *)((u8 *)self->worldVerts + worldOffset);
        }
        self->parts[partIdx] = part;
        partIdx++;
        worldOffset += 0xc0;
      }
      else if (chunk->tag == g_vertChunkTag) {
        part->vertices = (const ScePspFVector4 *)data;
      }
      else if (chunk->tag == g_normChunkTag) {
        part->normals = (const ScePspFVector4 *)data;
      }
      else if (chunk->tag == g_faceChunkTag) {
        part->faces = (const u16 *)data;
      }
      else if (chunk->tag == g_geomChunkTag) {
        part->localMatrix = (const ScePspFMatrix4 *)data;
        if ((file->flags & 0x8000) == 0) {
          /* File form: x = translation, y = rotation angles (radians), z = scale. */
          geom = (ScePspFMatrix4 *)part->localMatrix;
          sx = geom->z.x;
          sy = geom->z.y;
          sz = geom->z.z;
          /* vrot on angle * 2/pi (bank S703) in quarter turns: cos/sin of the angle. */
          rc = __builtin_cosf(geom->y.x);
          rs = __builtin_sinf(geom->y.x);
          /* Scale matrix times the X rotation. The Y and Z rotations take their angles from
             S101/S102 (off-diagonal zeros of the scale matrix), not from the Y/Z angles, so
             they are identity rotations: only the X angle is applied (original bug). */
          tmp.x.x = sx;
          tmp.x.y = 0.0f;
          tmp.x.z = 0.0f;
          tmp.x.w = 0.0f;
          tmp.y.x = 0.0f;
          tmp.y.y = rc * sy;
          tmp.y.z = rs * sy;
          tmp.y.w = 0.0f;
          tmp.z.x = 0.0f;
          tmp.z.y = -rs * sz;
          tmp.z.z = rc * sz;
          tmp.z.w = 0.0f;
          tmp.w = geom->x;
          tmp.w.w = 1.0f;
          *geom = tmp;
        }
      }
      else if (chunk->tag == g_aabbChunkTag) {
        aabb = (CollisionBvhPart *)data;
        if ((file->flags & 0x8000) == 0) {
          aabb->nodes = (CollisionBvhNode *)(data + aabb->nodeBase);
          aabb->tris = data + aabb->triBase;
        }
        part->bvhUnk18 = aabb->unk00;
        part->bvhRoot = aabb->nodes;
        part->rotation = (const float *)0;
        part->bvhTris = aabb->tris;
        if ((file->flags & 0x8000) == 0) {
          CollisionBvhRelocate(part->bvhRoot, aabb);
        }
      }
      chunkIdx++;
      chunk = (CollisionFileChunk *)(data + (((chunk->sizeAndPad & 0xffffff) + 3) & ~3u));
    } while (chunkIdx < file->chunkCount);
  }
  file->flags = file->flags | 0x8000;
  return;
}
