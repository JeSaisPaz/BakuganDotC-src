// bdc 0x089cba3c ScriptMngUpdate
#include "bdc.h"

/* Per-frame update of the script system, called from `BootMainThread` as the loop's liveness
   check. Steps every script of `g_scriptList` with `ScriptStep` (saving the `next` pointer
   first); a script that returns -1 is unlinked if it is the list head and destroyed through its
   vtable destructor (slot at `+0xc`, argument 3). Returns the number of scripts visited, so 0 means
   no script is left and the main loop shuts down. If the restart flag `g_scriptRestartRequest` is
   set it instead reloads the package (when `g_scriptReloadPath` is set, `ScriptPackageLoad`),
   calls `ScriptRestartBoot`, clears the flag and returns 1; if the pause flag `g_scriptPaused`
   is set it skips stepping and returns 1. */

int ScriptMngUpdate(void *mng)

{
  Script *script;
  Script *next;
  int count;

  count = 0;
  next = g_scriptList;
  if (g_scriptRestartRequest != 0) {
    if (g_scriptReloadPath != (const char *)0x0) {
      ScriptPackageLoad(g_scriptReloadPath);
    }
    ScriptRestartBoot(); /* asm also forwards mng in a0, unused */
    g_scriptRestartRequest = 0;
    return 1;
  }
  if (g_scriptPaused != 0) {
    return 1;
  }
  while ((script = next) != (Script *)0x0) {
    next = script->next;
    count++;
    if (ScriptStep(script) == -1) {
      if (script == g_scriptList) {
        g_scriptList = next;
      }
      if (script != (Script *)0x0) {
        const VtblEntry *dtor = (const VtblEntry *)script->vtable + 1;
        ((void (*)(void *, int))dtor->fn)((u8 *)script + dtor->delta, 3);
      }
    }
  }
  return count;
}
