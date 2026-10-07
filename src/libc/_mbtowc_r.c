// bdc 0x089bacd8 _mbtowc_r
#include "bdc.h"

/* Newlib's `_mbtowc_r(reent, pwc, s, n, state)`: converts the multibyte character at `s` (at most
   `n` bytes) to a 16-bit wide character stored in `*pwc` (a local dummy when `pwc` is NULL),
   returning the number of bytes consumed, 0 for a NUL byte or `s == NULL`, or -1 for an
   invalid/incomplete sequence. The charset is the name in `reent->_current_locale`:
   "C-SJIS", "C-EUCJP", "C-JIS" (stateful ISO-2022-JP using `g_jisStateTable` /
   `g_jisActionTable` and `*state`), anything else single bytes. */

/* JIS decoder character classes (columns of the JIS tables). */
enum {
    JIS_C_ESCAPE,
    JIS_C_DOLLAR,
    JIS_C_BRACKET,
    JIS_C_AT,
    JIS_C_B,
    JIS_C_J,
    JIS_C_NUL,
    JIS_C_CHAR,
    JIS_C_OTHER
};

/* JIS decoder states used as start rows. */
enum {
    JIS_S_ASCII = 0,
    JIS_S_JIS = 3
};

/* JIS decoder actions. */
enum {
    JIS_A_COPY_A,
    JIS_A_COPY_J1,
    JIS_A_COPY_J2,
    JIS_A_MAKE_A,
    JIS_A_MAKE_J,
    JIS_A_NOOP,
    JIS_A_EMPTY
};

static int JisCharClass(u8 ch)
{
    switch (ch) {
    case 0x1b: return JIS_C_ESCAPE;
    case '$': return JIS_C_DOLLAR;
    case '(': return JIS_C_BRACKET;
    case '@': return JIS_C_AT;
    case 'B': return JIS_C_B;
    case 'J': return JIS_C_J;
    case '\0': return JIS_C_NUL;
    default:
        if (ch > 0x20 && ch < 0x7f) {
            return JIS_C_CHAR;
        }
        return JIS_C_OTHER;
    }
}

int _mbtowc_r(_reent *reent, u16 *pwc, u8 *s, u32 n, s32 *state)
{
    u16 dummy;
    const char *charset;

    if (pwc == NULL) {
        pwc = &dummy;
    }
    if (s != NULL && n == 0) {
        return -1;
    }
    charset = reent->_current_locale;
    if (charset != NULL && (int)strlen(charset) > 1) {
        if (strcmp(reent->_current_locale, "C-SJIS") == 0) {
            u8 lead;

            if (s == NULL) {
                return 0;
            }
            lead = s[0];
            if ((lead > 0x80 && lead < 0xa0) || (lead > 0xdf && lead < 0xf0)) {
                u8 trail;

                if (n < 2) {
                    return -1;
                }
                trail = s[1];
                if ((trail >= 0x40 && trail <= 0x7e) || (trail >= 0x80 && trail <= 0xfc)) {
                    *pwc = (u16)(lead * 0x100 + trail);
                    return 2;
                }
                return -1;
            }
        } else if (strcmp(reent->_current_locale, "C-EUCJP") == 0) {
            u8 lead;

            if (s == NULL) {
                return 0;
            }
            lead = s[0];
            if (lead > 0xa0 && lead != 0xff) {
                u8 trail;

                if (n < 2) {
                    return -1;
                }
                trail = s[1];
                if (trail > 0xa0 && trail != 0xff) {
                    *pwc = (u16)(lead * 0x100 + trail);
                    return 2;
                }
                return -1;
            }
        } else if (strcmp(reent->_current_locale, "C-JIS") == 0) {
            const u8 *ptr = s; /* start of the current character */
            const u8 *t = s;
            int curState;
            u32 i;

            if (s == NULL) {
                *state = 0;
                return 1;
            }
            curState = (*state == 0) ? JIS_S_ASCII : JIS_S_JIS;
            for (i = 0; i < n; i++, t++) {
                int cls = JisCharClass(*t);
                s32 action = g_jisActionTable[curState][cls];

                curState = g_jisStateTable[curState][cls];
                switch (action) {
                case JIS_A_COPY_A:
                    *state = 0;
                    *pwc = ptr[0];
                    return (int)(i + 1);
                case JIS_A_COPY_J1:
                    *state = 0;
                    *pwc = (u16)(ptr[0] * 0x100 + ptr[1]);
                    return (int)(i + 1);
                case JIS_A_COPY_J2:
                    *state = 1;
                    *pwc = (u16)(ptr[0] * 0x100 + ptr[1]);
                    return (int)(ptr - s) + 2;
                case JIS_A_MAKE_A:
                case JIS_A_MAKE_J:
                    ptr = t + 1;
                    break;
                case JIS_A_NOOP:
                    break;
                case JIS_A_EMPTY:
                    *state = 0;
                    *pwc = 0;
                    return (int)i;
                default:
                    return -1;
                }
            }
            return -1;
        }
    }
    if (s == NULL) {
        return 0;
    }
    *pwc = s[0];
    return s[0] != '\0';
}
