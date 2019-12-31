#include <cstdint>
#define STRING(s) #s

// an enumeration can also be used here
template <typename T, T value>
constexpr T ensure_constexpr() {
  return value;
}
#define CONSTEXPR(x) ensure_constexpr<decltype(x), x>()

constexpr uint64_t fnv1impl(uint64_t h, const char* s) {
  return (*s == 0)
             ? h
             : fnv1impl((h * 1099511628211ull) ^ static_cast<uint64_t>(*s),
                        s + 1);
}

constexpr uint64_t fnv1(const char* s) {
  return fnv1impl(14695981039346656037ull, s);
}

template <uint64_t n>
constexpr uint64_t get_seed(const uint64_t x,
                            const uint64_t y = CONSTEXPR(fnv1(STRING(n)))) {
  return x ^ y * n;
}

#define SEED CONSTEXPR(get_seed<__COUNTER__ + 1>(fnv1(__TIME__)))

//! Rotate left by
//! \param x
//! \param k
//! \return
constexpr uint64_t rotl(uint64_t x, int k) {
  return (x << k) | (x >> (64 - k));
}

//! XorShift 256 compile time random implementation
//! \tparam a
//! \tparam b
//! \tparam c
//! \tparam d
//! \return
template <uint64_t a, uint64_t b, uint64_t c, uint64_t d>
constexpr uint64_t xorshift256_next() {
  uint64_t s[4] = {a, b, c, d};
  const uint64_t t = s[1] << 17;

  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];

  s[2] ^= t;

  s[3] = rotl(s[3], 45);

  return rotl(s[0] + s[3], 23) + s[0];
}

#define RND                                                         \
  xorshift256_next<                                                 \
      CONSTEXPR(SEED + __COUNTER__), CONSTEXPR(SEED + __COUNTER__), \
      CONSTEXPR(SEED + __COUNTER__), CONSTEXPR(SEED + __COUNTER__)>()