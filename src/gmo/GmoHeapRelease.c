// bdc 0x08a13214 GmoHeapRelease
#include "bdc.h"

/* Drops a reference to the block of the model library's 3-pool block heap (`g_gmoHeapPools`; same
   design as the image library's `GmoImageHeapAllocBlock` heap) that holds `ptr`, searching pool
   `pool` first and then the next pools (mod 3), with the same cache0/cache1/range+list lookup as
   `GmoHeapAddRef`. A block whose count reaches 0 is unlinked from the pool named in the block
   (fixing the block list and lookup cache) and either parked on the pool's `freeList` (pool flag
   set) or freed with the pool's free function. Returns `ptr` (NULL and unknown pointers are passed
   through). */

void *GmoHeapRelease(int pool, void *ptr)
{
  u8 *addr = (u8 *)ptr;
  GmoImagePool *p;
  GmoImageBlock *c0;
  GmoImageBlock *c1;
  GmoImageBlock *b;
  GmoImageBlock *next;
  GmoImageBlock *prev;
  u32 head;
  u32 tail;
  u16 count;
  int i;

  if (ptr == NULL) {
    return ptr;
  }
  for (i = 0; i != 3; i++) {
    p = &g_gmoHeapPools[pool];
    c0 = p->cache0;
    if (c0 != NULL && addr < (u8 *)c0 && (u8 *)c0->alloc <= addr) {
      b = c0;
      goto release;
    }
    c1 = p->cache1;
    if (c1 != NULL && addr < (u8 *)c1 && (u8 *)c1->alloc <= addr) {
      b = c1;
      goto found;
    }
    if (!(addr < p->rangeLow) && addr < p->rangeHigh) {
      for (b = p->blocks; b != NULL; b = b->prev) {
        if (addr < (u8 *)b && (u8 *)b->alloc <= addr) {
          goto found;
        }
      }
    }
    pool = (pool + 1) % 3;
  }
  return ptr;

found:
  p->cache1 = c0;
  p->cache0 = b;
release:
  count = (u16)(b->refCount - 1);
  b->refCount = (s16)count;
  if (count != 0) {
    return ptr;
  }
  p = &g_gmoHeapPools[b->pool];
  next = b->next;
  if (next != NULL) {
    next->prev = b->prev;
  }
  prev = b->prev;
  if (prev != NULL) {
    prev->next = next;
  }
  if (p->blocks == b) {
    p->blocks = prev;
  }
  if (p->cache1 == b) {
    p->cache1 = NULL;
  }
  if (p->cache0 == b) {
    next = p->cache1;
    p->cache1 = NULL;
    p->cache0 = next;
  }
  if (p->flag != 0) {
    head = g_gmoHeapBlockFreeHead;
    tail = g_gmoHeapBlockFreeTail;
    memcpy(&b->prev, &head, 4);
    b->next = p->freeList;
    b->pool = (u8)tail;
    b->offset = (u8)(tail >> 8);
    b->refCount = (s16)(tail >> 16);
    p->freeList = b;
    return ptr;
  }
  p->free(b->alloc);
  return ptr;
}
