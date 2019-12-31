#ifndef TESTS_UTILS_H

#define TESTS_UTILS_H
#if defined(_MSC_VER)
#define FORCEINLINE __forceinline
#endif

#if !defined(FORCEINLINE) && (defined(__GNUC__) || defined(__clang__))
#define FORCEINLINE __attribute__((always_inline)) inline
#endif

#endif
