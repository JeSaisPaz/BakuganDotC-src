// bdc 0x089b522c strtok_r
#include "bdc.h"

/* Standard `strtok_r` (newlib `__strtok_r` with leading-delimiter skipping): returns the next token
   of `s` (or of `*save` when `s` is NULL) delimited by any character of `delim`, NUL-terminates it
   and stores the resume point in `*save` (NULL after the last token). Returns NULL when no token
   is left. */

char *strtok_r(char *s, const char *delim, char **save)
{
    const char *d;
    char *tok;
    char *end;
    char c;
    char dc;

    if (s == NULL && (s = *save) == NULL) {
        return NULL;
    }

skip:
    tok = s;
    c = *s++;
    for (d = delim; (dc = *d++) != '\0';) {
        if (c == dc) {
            goto skip;
        }
    }
    if (c == '\0') {
        *save = NULL;
        return NULL;
    }

    for (;;) {
        end = s;
        c = *s++;
        d = delim;
        do {
            if ((dc = *d++) == c) {
                if (c == '\0') {
                    s = NULL;
                } else {
                    *end = '\0';
                }
                *save = s;
                return tok;
            }
        } while (dc != '\0');
    }
}
