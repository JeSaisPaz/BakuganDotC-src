// bdc 0x08a19844 GmoVertexArrayRelease
#include "bdc.h"

/* Drops a reference on a mesh's vertex-array record (`mesh+0xc`); at zero frees its data block
   (`+4`) and the record (`GmoHeapRelease`). Returns `arr`. */

void *GmoVertexArrayRelease(void *arr)
{
  GmoVertexArray *va = (GmoVertexArray *)arr;

  if (va != NULL) {
    va->refCount = va->refCount - 1;
    if (va->refCount == 0) {
      GmoHeapReleaseThunk(0, va->data);
      GmoHeapReleaseThunk(0, va);
    }
  }
  return arr;
}
