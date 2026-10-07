// bdc 0x089b51a0 strstr
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `strstr` at US `0x089b51e0`. */

/* Returns the first occurrence of `needle` in `haystack`, or NULL. An empty
   `haystack` matches only an empty `needle`. */
char *strstr(const char *haystack, const char *needle)
{
    if (*haystack == '\0') {
        return *needle == '\0' ? (char *)haystack : NULL;
    }
    do {
        int i = 0;

        while (needle[i] != '\0') {
            if (needle[i] != haystack[i]) {
                break;
            }
            i++;
        }
        if (needle[i] == '\0') {
            return (char *)haystack;
        }
        haystack++;
    } while (*haystack != '\0');
    return NULL;
}
