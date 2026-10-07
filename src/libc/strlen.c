// bdc 0x089b50e0 strlen
#include "bdc.h"

/* Standard `strlen`: counts the bytes of `s` before its terminating NUL. */
size_t strlen(const char *s)
{
    const char *end = s;

    while (*end != '\0') {
        end++;
    }
    return (size_t)(end - s);
}
