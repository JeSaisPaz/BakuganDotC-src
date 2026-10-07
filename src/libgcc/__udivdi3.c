// bdc 0x08a0d7c0 __udivdi3
#include "bdc.h"

/* libgcc `__udivdi3`: unsigned 64-bit division (quotient), schoolbook 2-by-1 / 3-by-2 word division
   normalised with the `__clz_tab` count-leading-zeros table at `0x08aa5018`. Used by
   `_vfprintf_r`/`_vfiprintf_r` to print `%lld`/`%llu` digits.
   This is libgcc2.c `__udivmoddi4` (UDIV_NEEDS_NORMALIZATION) with the remainder dropped, built
   from the generic C `count_leading_zeros` / `udiv_qrnnd` / `umul_ppmm` of `longlong.h`. A zero
   divisor traps (`break 7`) in the first `divu`. */

/* longlong.h `count_leading_zeros` (table version): number of leading zero bits of the non-zero
   32-bit `x`. */
#define COUNT_LEADING_ZEROS(count, x)                                                            \
  do {                                                                                           \
    u32 clzX_ = (x);                                                                             \
    u32 clzA_ = clzX_ < 0x10000 ? (clzX_ < 0x100 ? 0 : 8) : (clzX_ < 0x1000000 ? 16 : 24);       \
    (count) = 32 - (__clz_tab[clzX_ >> clzA_] + clzA_);                                          \
  } while (0)

/* longlong.h `__udiv_qrnnd_c`: divides the 64-bit (`n1`:`n0`) by the normalised `d` (top bit set,
   `n1 < d`) in two 16-bit digit steps; quotient to `q`, remainder to `r`. */
#define UDIV_QRNND(q, r, n1, n0, d)                                                              \
  do {                                                                                           \
    u32 udD_ = (d);                                                                              \
    u32 udD1_ = udD_ >> 16;                                                                      \
    u32 udD0_ = udD_ & 0xffff;                                                                   \
    u32 udN0_ = (n0);                                                                            \
    u32 udN1_ = (n1);                                                                            \
    u32 udQ1_ = udN1_ / udD1_;                                                                   \
    u32 udR1_ = udN1_ % udD1_;                                                                   \
    u32 udM_ = udQ1_ * udD0_;                                                                    \
    u32 udQ0_;                                                                                   \
    u32 udR0_;                                                                                   \
    udR1_ = (udR1_ << 16) | (udN0_ >> 16);                                                       \
    if (udR1_ < udM_) {                                                                          \
      udQ1_--;                                                                                   \
      udR1_ += udD_;                                                                             \
      if (udR1_ >= udD_ && udR1_ < udM_) {                                                       \
        udQ1_--;                                                                                 \
        udR1_ += udD_;                                                                           \
      }                                                                                          \
    }                                                                                            \
    udR1_ -= udM_;                                                                               \
    udQ0_ = udR1_ / udD1_;                                                                       \
    udR0_ = udR1_ % udD1_;                                                                       \
    udM_ = udQ0_ * udD0_;                                                                        \
    udR0_ = (udR0_ << 16) | (udN0_ & 0xffff);                                                    \
    if (udR0_ < udM_) {                                                                          \
      udQ0_--;                                                                                   \
      udR0_ += udD_;                                                                             \
      if (udR0_ >= udD_ && udR0_ < udM_) {                                                       \
        udQ0_--;                                                                                 \
        udR0_ += udD_;                                                                           \
      }                                                                                          \
    }                                                                                            \
    udR0_ -= udM_;                                                                               \
    (q) = (udQ1_ << 16) | udQ0_;                                                                 \
    (r) = udR0_;                                                                                 \
  } while (0)

unsigned long long __udivdi3(unsigned long long a, unsigned long long b)
{
  u32 n1 = (u32)(a >> 32);
  u32 n0 = (u32)a;
  u32 d1 = (u32)(b >> 32);
  u32 d0 = (u32)b;
  u32 n2;
  u32 q0;
  u32 q1;
  u32 bm;
  u32 shift;

  if (d1 == 0) {
    if (d0 > n1) {
      /* 0q = nn / 0D */
      COUNT_LEADING_ZEROS(bm, d0);
      if (bm != 0) {
        d0 <<= bm;
        n1 = (n1 << bm) | (n0 >> (32 - bm));
        n0 <<= bm;
      }
      UDIV_QRNND(q0, n0, n1, n0, d0);
      q1 = 0;
    } else {
      /* qq = NN / 0d */
      if (d0 == 0) {
        d0 = 1 / d0; /* divide by zero on purpose: traps */
      }
      COUNT_LEADING_ZEROS(bm, d0);
      if (bm == 0) {
        /* d0 normalised and n1 >= d0: the high quotient word is 1. */
        n1 -= d0;
        q1 = 1;
      } else {
        shift = 32 - bm;
        d0 <<= bm;
        n2 = n1 >> shift;
        n1 = (n1 << bm) | (n0 >> shift);
        n0 <<= bm;
        UDIV_QRNND(q1, n1, n2, n1, d0);
      }
      UDIV_QRNND(q0, n0, n1, n0, d0);
    }
  } else {
    if (d1 > n1) {
      /* 00 = nn / DD */
      q0 = 0;
      q1 = 0;
    } else {
      /* 0q = NN / dd */
      COUNT_LEADING_ZEROS(bm, d1);
      if (bm == 0) {
        /* d1 normalised and n1 >= d1: the quotient is 1 iff (n1:n0) >= (d1:d0). */
        q0 = (n1 > d1 || n0 >= d0) ? 1 : 0;
        q1 = 0;
      } else {
        u64 m;

        shift = 32 - bm;
        d1 = (d1 << bm) | (d0 >> shift);
        d0 <<= bm;
        n2 = n1 >> shift;
        n1 = (n1 << bm) | (n0 >> shift);
        n0 <<= bm;
        UDIV_QRNND(q0, n1, n2, n1, d1);
        m = (u64)q0 * d0;
        if ((u32)(m >> 32) > n1 || ((u32)(m >> 32) == n1 && (u32)m > n0)) {
          q0--;
        }
        q1 = 0;
      }
    }
  }

  return ((u64)q1 << 32) | q0;
}
