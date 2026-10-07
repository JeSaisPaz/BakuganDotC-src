// bdc 0x089beffc GfxCombSortByDepth
#include "bdc.h"

/* Sorts an array of `count` 8-byte `{item, float depth}` pairs in descending order of `depth` with
   a comb sort (gap shrinks by 7/8 until 1 and a pass makes no swap). Used to order models,
   particles and render packets back-to-front. */

typedef struct DepthPair {
  void *item;
  float depth;
} DepthPair;

void GfxCombSortByDepth(void *pairs, u32 count)
{
  DepthPair *a = (DepthPair *)pairs;
  u32 gap = count;
  bool swapped;
  u32 i;

  if (count < 2) {
    return;
  }
  do {
    gap = (gap * 7) >> 3;
    swapped = false;
    for (i = 0; i + gap < count; i++) {
      if (a[i].depth < a[i + gap].depth) {
        DepthPair t = a[i];
        a[i] = a[i + gap];
        a[i + gap] = t;
        swapped = true;
      }
    }
  } while (swapped || gap > 1);
}
