// bdc 0x089cbb44 ScriptRestartBoot
#include "bdc.h"

/* Destroys every script in `g_scriptList` (walking the `+0x4` next chain and calling each vtable
   destructor with 3), empties the list and then respawns the entry script `"program/boot.lzs"` with
   `ScriptSpawn` — a soft restart of the game's script layer. */

void ScriptRestartBoot(void)

{
  Script *script;
  Script *next;

  if (g_scriptList != (Script *)0x0) {
    next = g_scriptList->next;
    script = g_scriptList;
    while (1) {
      if (script != (Script *)0x0) {
        const VtblEntry *dtor = (const VtblEntry *)script->vtable + 1;
        ((void (*)(void *, int))dtor->fn)((u8 *)script + dtor->delta, 3);
      }
      script = next;
      if (script == (Script *)0x0) break;
      next = script->next;
    }
    g_scriptList = (Script *)0x0;
  }
  ScriptSpawn("program/boot.lzs");
  return;
}
