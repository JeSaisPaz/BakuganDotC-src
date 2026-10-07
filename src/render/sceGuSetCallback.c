// bdc 0x08a1ee38 sceGuSetCallback
#include "bdc.h"

/* libgu `sceGuSetCallback(signal, callback)`: stores the callback for signal 1 (GE SIGNAL,
   g_guSignalCallback) or 4 (GE FINISH, g_guFinishCallback) and returns the previous one;
   other values return 0. */

void *sceGuSetCallback(s32 signal, void *callback)
{
  void *old = (void *)0;

  if (signal == 1) {
    old = g_guSignalCallback;
    g_guSignalCallback = callback;
  } else if (signal == 4) {
    old = g_guFinishCallback;
    g_guFinishCallback = callback;
  }
  return old;
}
