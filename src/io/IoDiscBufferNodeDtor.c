// bdc 0x08a324e4 IoDiscBufferNodeDtor
#include "bdc.h"

/* Destructor (vtable `0x08af7048` entry 1) of the read-buffer node of the disc reader
   (`IoDiscSimpleCtor` builds two at `+0x38`/`+0x3c`): frees the 0x30000-byte aligned buffer
   `+0x24` (`MemFreeAligned`), runs `CoreNodeDtor` and frees the node when `flags & 1`. */

void IoDiscBufferNodeDtor(CoreNode *node, u32 flags)

{
  IoDiscBufNode *buf = (IoDiscBufNode *)node;
  unsigned char *ptr;
  
  if (node != (CoreNode *)0x0) {
    ptr = buf->data;
    node->vtable = g_ioDiscBufNodeVtbl;
    MemFreeAligned(ptr);
    CoreNodeDtor(node,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(node,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

