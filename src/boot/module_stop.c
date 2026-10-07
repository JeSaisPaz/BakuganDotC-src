// bdc 0x089b4778 module_stop
#include "bdc.h"

/* PRX `module_stop` export of the game module (export table at `0x08a33400`: NIDs 0xD632ACDB
   `module_start` = `BootEntry`, 0xCEE8593C `module_stop` = this): calls the weak function pointer
   `0x08a9fb98` (`__psp_free_heap`) when it is linked, then returns 0. */

/* The SDK crt0 declares the hook weak, so its address test survives in the asm
   (`addiu a0,v1,-0x468; beq a0,zero`); a note `type:` cannot carry `weak`, hence this redeclaration.
   In this link the address is 0x08a9fb98 (never 0), so the hook is always called. */
extern void *g_pspFreeHeapHook __attribute__((weak));

int module_stop(SceSize args, void *argp)
{
    if (&g_pspFreeHeapHook != NULL) {
        ((void (*)(void))g_pspFreeHeapHook)();
    }
    return 0;
}
