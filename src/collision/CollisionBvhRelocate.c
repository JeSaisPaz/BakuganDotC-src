// bdc 0x089e39a4 CollisionBvhRelocate
#include "bdc.h"

/* Recursively turns the file-relative offsets of a collision BVH into pointers: inner nodes (`+0x20
   == 0`) get their child offsets `+0x28`/`+0x2c` rebased by the part's node base (`part+4`) and are
   recursed into; leaves rebase their triangle index list `+0x24` by `part+8`. */

void CollisionBvhRelocate(CollisionBvhNode *node, const CollisionBvhPart *part)

{
  if (node->kind == 0) {
    node->left = node->left + part->nodes;
    node->right = node->right + part->nodes;
    CollisionBvhRelocate((CollisionBvhNode *)PspPtr(node->left), part);
    CollisionBvhRelocate((CollisionBvhNode *)PspPtr(node->right), part);
  }
  else {
    node->tri = node->tri + part->tris;
  }
  return;
}
