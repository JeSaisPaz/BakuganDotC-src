// bdc 0x089b43a0 BootEntry
#include "bdc.h"

/* The module's real entry point (ELF `e_entry` = `0x089b43a0`), the crt0 start function with no
   callers: reports the SDK and compiler versions (`sceKernelSetCompiledSdkVersion600_602`
   `0x6020010`, `sceKernelSetCompilerVersion` `0x30306`), creates the `"user_main"` thread running
   `BootUserMainThread` with priority `g_bootUserMainPriority` (`0x20`), a
   `g_bootUserMainStackKb` KiB stack (40 KiB, `<< 10`) and attribute `0x80000000` (user mode),
   and starts it with this function's own `(args, argp)`. Returns 0; the game proper then runs in
   `main` on the new thread.
   The SDK crt0 tests each weak tuning symbol's address before reading it (stack-cookie seed,
   thread name, attribute); those symbols are unlinked (address 0) here, so the asm keeps only the
   defaults shown below and the null-address branches are dead. */

int BootEntry(SceSize args, void *argp)
{
  SceUID thid;

  sceKernelSetCompiledSdkVersion600_602(0x6020010);
  sceKernelSetCompilerVersion(0x30306);
  thid = sceKernelCreateThread("user_main", BootUserMainThread, g_bootUserMainPriority,
                               g_bootUserMainStackKb << 10, 0x80000000, NULL);
  sceKernelStartThread(thid, args, argp);
  return 0;
}
