#ifndef VM_NAND_NOR_H
#define VM_NAND_NOR_H
#include "utils.h"
//------------------------------------------------------------------------------
template <typename T>
FORCEINLINE T Not(volatile T a) {
  return ~a;
}

template <typename T>
FORCEINLINE T Or(volatile T a, volatile T b) {
  return a | b;
}

template <typename T>
FORCEINLINE T And(volatile T a, volatile T b) {
  return a & b;
}
//------------------------------------------------------------------------------
//! Not (a Or b)
template <typename T>
FORCEINLINE volatile T Nor(volatile T a, volatile T b) {
  return Not<T>(Or<T>(a, b));
}
//! Not (a And b)
template <typename T>
FORCEINLINE volatile T Nand(volatile T a, volatile T b) {
  return Not<T>(And<T>(a, b));
}
//! (a Nand (a Nand b)) Nand (b Nand (a Nand b))
template <typename T>
FORCEINLINE T Xor(volatile T a, volatile T b) {
  return Nand<T>(Nand<T>(a, Nand<T>(a, b)), (Nand<T>(b, Nand<T>(a, b))));
}
//------------------------------------------------------------------------------
//! (Not a) Or (Not b)
template <typename T>
FORCEINLINE T Nand_1(volatile T a, volatile T b) {
  return Or<T>(Not<T>(a), Not<T>(b));
}
//! Not(a And b)
template <typename T>
FORCEINLINE T Nand_2(volatile T a, volatile T b) {
  return Not<T>(And<T>(a, b));
}
//! Not ((Not a) Nor (Not b))
template <typename T>
FORCEINLINE T Nand_3(volatile T a, volatile T b) {
  return Not<T>(Nor<T>(Not<T>(a), Not<T>(b)));
}
//! (Not a) Or (Not b)
template <typename T>
FORCEINLINE T Nand_4(volatile T a, volatile T b) {
  return Or<T>(Not<T>(a), Not<T>(b));
}
//! ((Not a) And b) Xor (Not b)
template <typename T>
FORCEINLINE T Nand_5(volatile T a, volatile T b) {
  return Xor<T>(And<T>(Not<T>(a), b), Not<T>(b));
}
//------------------------------------------------------------------------------
//! (Not a) And (Not b)
template <typename T>
FORCEINLINE T Nor_1(volatile T a, volatile T b) {
  return And<T>(Not<T>(a), Not<T>(b));
}
//! Not(a Xor b Xor (a And b))
template <typename T>
FORCEINLINE T Nor_2(volatile T a, volatile T b) {
  return Not<T>(Xor<T>(Xor<T>(a, b), And<T>(a, b)));
}
//! Not((Not a) Nand (Not b))
template <typename T>
FORCEINLINE T Nor_3(volatile T a, volatile T b) {
  return Not<T>(Nand<T>(Not<T>(a), Not<T>(b)));
}
//! (Not a) And (Not b)
template <typename T>
FORCEINLINE T Nor_4(volatile T a, volatile T b) {
  return And<T>(Not<T>(a), Not<T>(b));
}
//! Not(a Or b)
template <typename T>
FORCEINLINE T Nor_5(volatile T a, volatile T b) {
  return Not<T>(Or<T>(a, b));
}
//------------------------------------------------------------------------------
template <typename T, const uint64_t n>
FORCEINLINE volatile T NandR(volatile T a, volatile T b) {
  switch (n % 5) {
    case 0:
      return Nand_1<T>(a, b);
    case 1:
      return Nand_2<T>(a, b);
    case 2:
      return Nand_3<T>(a, b);
    case 3:
      return Nand_4<T>(a, b);
    case 4:
      return Nand_5<T>(a, b);
    default:
      return Nand_1<T>(a, b);
  }
}
//------------------------------------------------------------------------------
template <typename T, const uint64_t n>
FORCEINLINE volatile T NorR(volatile T a, volatile T b) {
  switch (n % 5) {
    case 0:
      return Nor_1<T>(a, b);
    case 1:
      return Nor_2<T>(a, b);
    case 2:
      return Nor_3<T>(a, b);
    case 3:
      return Nor_4<T>(a, b);
    case 4:
      return Nor_5<T>(a, b);
    default:
      return Nor_1<T>(a, b);
  }
}
//------------------------------------------------------------------------------
template <typename T, const uint64_t n>
FORCEINLINE volatile T NotR(volatile uint64_t a) {
  switch (n % 10) {
    case 0:
      return Nand_1<T>(a, a);
    case 1:
      return Nand_2<T>(a, a);
    case 2:
      return Nand_3<T>(a, a);
    case 3:
      return Nand_4<T>(a, a);
    case 4:
      return Nand_5<T>(a, a);
    case 5:
      return Nor_1<T>(a, a);
    case 6:
      return Nor_2<T>(a, a);
    case 7:
      return Nor_3<T>(a, a);
    case 8:
      return Nor_4<T>(a, a);
    case 9:
      return Nor_5<T>(a, a);
    default:
      return Nand_1<T>(a, a);
  }
}
//------------------------------------------------------------------------------
template <typename T, const uint64_t n>
FORCEINLINE volatile T AndR(volatile T a, volatile T b) {
  switch (n % 5) {
    // (Not a) Nor (Not b)
    case 0:
      return NorR<T, n + __COUNTER__>(NotR<T, n + __COUNTER__>(a),
                                      NotR<T, n + __COUNTER__>(b));
    // Not (a Nand b)
    case 1:
      return NotR<T, n + __COUNTER__>(NandR<T, n + __COUNTER__>(a, b));
    // Not((Not a) Or (Not b))
    case 2:
      return NotR<T, n + __COUNTER__>(
          Or(NotR<T, n + __COUNTER__>(a), NotR<T, n + __COUNTER__>(b)));
    // (a Xor (Not b)) And a
    case 3:
      return And<T>(a, Xor<T>(a, NotR<T, n + __COUNTER__>(b)));
      // Not((Not a) Or (Not b))
    case 4:
      return Not<T>(
          Or<T>(NotR<T, n + __COUNTER__>(a), NotR<T, n + __COUNTER__>(b)));

    default:
      return NorR<T, n + __COUNTER__>(NotR<T, n + __COUNTER__>(a),
                                      NotR<T, n + __COUNTER__>(b));
  }
}
//------------------------------------------------------------------------------
template <typename T, const uint64_t n>
FORCEINLINE volatile T OrR(volatile T a, volatile T b) {
  switch (n % 5) {
    // a Xor b Xor (a And b)
    case 0:
      return Xor<T>(Xor<T>(a, b), AndR<T, n + __COUNTER__>(a, b));
    // Not (a Nor b)
    case 1:
      return NotR<T, n + __COUNTER__>(NorR<T, n + __COUNTER__>(a, b));
    // (Not a) Nand (Not b)
    case 2:
      return NandR<T, n + __COUNTER__>(NotR<T, n + __COUNTER__>(a),
                                       NotR<T, n + __COUNTER__>(b));
    // Not((Not a) And (Not b))
    case 3:
      return NotR<T, n + __COUNTER__>(AndR<T, n + __COUNTER__>(
          NotR<T, n + __COUNTER__>(a), NotR<T, n + __COUNTER__>(b)));
    // b Xor (a And (Not b))
    case 4:
      return Xor<T>(b,
                    AndR<T, n + __COUNTER__>(a, NotR<T, n + __COUNTER__>(b)));

    default:
      return Xor<T>(Xor<T>(a, b), AndR<T, n + __COUNTER__>(a, b));
  }
}
//------------------------------------------------------------------------------
template <typename T, const uint64_t n>
FORCEINLINE volatile T XorR(volatile T a, volatile T b) {
  switch (n % 6) {
    // (a And (Not b)) Or ((Not a) And b)
    case 0:
      return OrR<T, n + __COUNTER__>(
          AndR<T, n + __COUNTER__>(a, (NotR<T, n + __COUNTER__>(b))),
          AndR<T, n + __COUNTER__>(NotR<T, n + __COUNTER__>(a), b));
    // ((Not a) Or (Not b)) And (a Or b)
    case 1:
      return And(OrR<T, n + __COUNTER__>(NotR<T, n + __COUNTER__>(a),
                                         NotR<T, n + __COUNTER__>(b)),
                 OrR<T, n + __COUNTER__>(a, b));
    // ((Not a) Nor (Not b)) Nor (a Nor b)
    case 2:
      return NorR<T, n + __COUNTER__>(
          NorR<T, n + __COUNTER__>(NotR<T, n + __COUNTER__>(a),
                                   NotR<T, n + __COUNTER__>(b)),
          NorR<T, n + __COUNTER__>(a, b));
    //  (a Nand (a Nand b)) Nand (b Nand (a Nand b))
    case 3:
      return NandR<T, n + __COUNTER__>(
          NandR<T, n + __COUNTER__>(a, NandR<T, n + __COUNTER__>(a, b)),
          NandR<T, n + __COUNTER__>(b, NandR<T, n + __COUNTER__>(a, b)));
    // (Not (a And b)) And (Not ((Not a) And (Not b)))
    case 4:
      return AndR<T, n + __COUNTER__>(
          (NotR<T, n + __COUNTER__>(AndR<T, n + __COUNTER__>(a, b))),
          NotR<T, n + __COUNTER__>(AndR<T, n + __COUNTER__>(
              NotR<T, n + __COUNTER__>(a), NotR<T, n + __COUNTER__>(b))));
    // (Not ((Not a) Or b)) Or (Not (a Or (Not b)))
    case 5:
      return OrR<T, n + __COUNTER__>(
          NotR<T, n + __COUNTER__>(
              OrR<T, n + __COUNTER__>(NotR<T, n + __COUNTER__>(a), b)),
          (NotR<T, n + __COUNTER__>(
              OrR<T, n + __COUNTER__>(a, NotR<T, n + __COUNTER__>(b)))));

    default:
      return OrR<T, n + __COUNTER__>(
          AndR<T, n + __COUNTER__>(a, (NotR<T, n + __COUNTER__>(b))),
          AndR<T, n + __COUNTER__>(NotR<T, n + __COUNTER__>(a), b));
  }
}

//------------------------------------------------------------------------------
template <typename T, const uint64_t n>
FORCEINLINE volatile T IncR(volatile T a) {
  switch (n % 1) {
    case 0: return -NotR<T, n + __COUNTER__>(a);

    default:
      return -NotR<T, n + __COUNTER__>(a);

  }
}
//------------------------------------------------------------------------------
template <typename T, const uint64_t n>
FORCEINLINE volatile T DecR(volatile T a) {
  switch (n % 1) {
    case 0: return NotR<T, n + __COUNTER__>(-a);

    default:
      return NotR<T, n + __COUNTER__>(-a);

  }
}

// TODO Add classic full adder/Kogge stone/Ripple carry/STA, LSHIFT, RSHIFT, CMP emulation (for comparison operators)

#endif
