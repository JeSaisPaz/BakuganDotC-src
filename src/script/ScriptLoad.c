// bdc 0x089c96fc ScriptLoad
#include "bdc.h"

/* Binds a named script to a script object: copies `name`, rewrites a `.script` extension to `.lzs`,
   looks the file up in the loaded package with `ScriptPackageFind` (the entry's name pointer is
   stored at `script+0x50`) and initialises the script's track tables from the decoded entry with
   `ScriptSetup`. */

void ScriptLoad(Script *script, const char *name)

{
  char *dst;
  void *entry;
  char path[128];
  
  strcpy(path,name);
  dst = strstr(path,".script");
  if (dst != (char *)0x0) {
    strcpy(dst,".lzs");
  }
  entry = ScriptPackageFind(path,&script->name);
  ScriptSetup(script,entry);
  return;
}

