// bdc 0x089cbbcc ScriptFindByName
#include "bdc.h"

/* Finds a running script by name: walks `g_scriptList` and returns the first `Script` whose
   name (`+0x50`) matches `name` case-insensitively (`strcasecmp`), or NULL. */

Script *ScriptFindByName(const char *name)
{
  Script *script;

  for (script = g_scriptList; script != (Script *)0x0; script = script->next) {
    if (strcasecmp(script->name, name) == 0) {
      return script;
    }
  }
  return (Script *)0x0;
}
