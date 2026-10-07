// bdc 0x089b44b4 BootUserMainThread
#include "bdc.h"

/* Entry of the `"user_main"` thread that the crt0 `BootEntry` creates and starts (its address is
   the thread-entry argument of `sceKernelCreateThread`); this is the C runtime start-up, not game
   logic. It splits the `args`-byte block of NUL-terminated strings `argp` into an `argv` array of
   at most 20 entries (using `strlen`), builds the thread's newlib `_reent` (`_reentCrt0`) on
   its stack and stores its address at `+4` of the `k0` thread pointer, registers `_fini` with
   `atexit`, runs `_init` and calls `main` with `(argc, argv)`. The result goes to `exit`,
   which runs the atexit handlers and does not return; the thread never returns normally.
   The weak-symbol alternatives of the SDK crt0 (address 0 in this image) are dead code and not
   reproduced. */

/* Platform hook (L7) for `sw reent, 4($k0)`: the port stores the user thread's _reent pointer. */
void PlatformSetThreadReent(void *reent);

int BootUserMainThread(SceSize args, void *argp)
{
    char *argv[24];
    _reentCrt0 reent;
    char *s;
    int argc;
    int i;
    unsigned int j;

    _sbrk(0);

    argc = 0;
    s = (char *)argp;
    if ((int)args > 0) {
        argv[0] = s;
        for (;;) {
            argc++;
            s = &s[strlen(s) + 1];
            if (argc >= 20)
                break;
            if (!((int)(s - (char *)argp) < (int)args))
                break;
            argv[argc] = s;
        }
    }
    argv[argc] = NULL;

    /* _REENT_INIT_PTR(&reent) */
    reent._stdout = (FILE *)reent.__sf[1];
    reent._stderr = (FILE *)reent.__sf[2];
    reent._errno = 0;
    reent._stdin = (FILE *)reent.__sf[0];
    reent._inc = 0;
    for (i = 0; i < 25; i++)
        reent._emergency[i] = 0;
    reent._current_locale = g_crt0LocaleC;
    reent._current_category = 0;
    reent.__sdidinit = 0;
    reent.__cleanup = NULL;
    reent._result = NULL;
    reent._result_k = 0;
    reent._p5s = NULL;
    reent._freelist = NULL;
    reent._cvtlen = 0;
    reent._cvtbuf = NULL;
    reent._unused_rand = 0;
    reent._strtok_last = NULL;
    reent._asctime_buf[0] = 0;
    for (j = 0; j < 36; j++)
        reent._localtime_buf[j] = 0;
    reent._rand_next_lo = 1;
    reent._rand_next_hi = 0;
    reent._r48_seed[0] = 0x330e;
    reent._r48_seed[1] = 0xabcd;
    reent._r48_seed[2] = 0x1234;
    reent._r48_mult[0] = 0xe66d;
    reent._r48_mult[1] = 0xdeec;
    reent._r48_mult[2] = 5;
    reent._r48_add = 0xb;
    reent._gamma_signgam = 0;
    reent._mblen_state[0] = 0;
    reent._mblen_state[1] = 0;
    reent._mbtowc_state[0] = 0;
    reent._mbtowc_state[1] = 0;
    reent._wctomb_state[0] = 0;
    reent._wctomb_state[1] = 0;
    reent._mbrlen_state[0] = 0;
    reent._mbrlen_state[1] = 0;
    reent._mbrtowc_state[0] = 0;
    reent._mbrtowc_state[1] = 0;
    reent._mbsrtowcs_state[0] = 0;
    reent._mbsrtowcs_state[1] = 0;
    reent._wcrtomb_state[0] = 0;
    reent._wcrtomb_state[1] = 0;
    reent._wcsrtombs_state[0] = 0;
    reent._wcsrtombs_state[1] = 0;
    reent._l64a_buf[0] = 0;
    reent._signal_buf[0] = 0;
    reent._getdate_err = 0;
    reent._atexit = NULL;
    reent._atexit0_next = NULL;
    reent._atexit0_ind = 0;
    reent._atexit0_fns[0] = NULL;
    reent._atexit0_fntypes = 0;
    reent._atexit0_fnargs[0] = NULL;
    reent._sig_func = NULL;
    reent.__sglue._next = NULL;
    reent.__sglue._niobs = 0;
    reent.__sglue._iobs = NULL;
    memset(reent._stdin, 0, sizeof(reent.__sf));
    reent.unk37c = 0;

    /* sw &reent, 4($k0): publish the thread's _reent at +4 of the kernel thread pointer (L7 hook). */
    PlatformSetThreadReent(&reent);

    atexit(_fini);
    _init();

    for (;;)
        exit(main(argc, argv));
}
