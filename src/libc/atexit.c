// bdc 0x089b4d10 atexit
#include "bdc.h"

/* Standard atexit (old newlib): appends fn to the current _atexit block of the global reent,
   linking in the static _atexit0 block on first use and pushing a freshly malloc'd block when the
   current one already holds 32 handlers. Returns 0, or -1 if that allocation fails. */
int atexit(void *fn)
{
  _reent *reent = g_impurePtr;
  _atexit *block = reent->_atexit;
  int slot;

  if (block == NULL) {
    block = &reent->_atexit0;
    reent->_atexit = block;
  }
  if (block->_ind >= 32) {
    block = malloc(sizeof(_atexit));
    if (block == NULL) {
      return -1;
    }
    block->_ind = 0;
    block->_next = g_impurePtr->_atexit;
    g_impurePtr->_atexit = block;
  }
  slot = block->_ind;
  block->_ind = slot + 1;
  block->_fns[slot] = fn;
  return 0;
}
