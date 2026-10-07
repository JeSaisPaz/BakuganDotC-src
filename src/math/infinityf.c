// bdc 0x08a0cf74 infinityf
#include "bdc.h"

/* newlib `infinityf()`: returns +infinity as a float (`0x7f800000`). Used by `__ieee754_powf`. */
float infinityf(void)
{
    return __builtin_inff();
}
