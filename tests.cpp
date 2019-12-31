#include <gtest/gtest.h>

#include <cstdint>
#include <functional>
#include <tuple>
#include <type_traits>
#include <vector>

#include "compile_random.h"
#include "nand_nor.h"
#include "nn_integer.h"

std::tuple<uint8_t, uint16_t, uint32_t, uint64_t> unsigned_max{
    UINT8_MAX, UINT16_MAX, UINT32_MAX, UINT64_MAX};
std::tuple<int8_t, int16_t, int32_t, int64_t> signed_min{INT8_MIN, INT16_MIN,
                                                         INT32_MIN, INT64_MIN};

// TODO Consider z3 for verification

enum EOperations {
  nor_,
  nand_,
  not_,
  and_,
  or_,
  xor_,
  add_,
  sub_,
  rshift_,
  lshift_
};

template <typename T>
std::remove_const_t<T> execute_operation(EOperations operation, T op1, T op2) {
  switch (operation) {
    case nor_:
      return ~(op1 | op2);
    case nand_:
      return ~(op1 & op2);
    case not_:
      return ~op1;
    case and_:
      return op1 & op2;
    case or_:
      return op1 | op2;
    case xor_:
      return op1 ^ op2;
    case add_:
      return op1 + op2;
    case sub_:
      return op1 - op2;
    case lshift_:
      return op1 << op2;
    case rshift_:
      return op1 >> op2;
    default:
      return 0;
  }
}

template <typename T>
std::vector<std::function<T(T, T)>> get_nor_primitives() {
  std::vector<std::function<T(T, T)>> primitives;
  primitives.emplace_back(
      std::bind(&Nor_1<T>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&Nor_2<T>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&Nor_3<T>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&Nor_4<T>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&Nor_5<T>, std::placeholders::_1, std::placeholders::_2));
  return primitives;
}

template <typename T>
std::vector<std::function<T(T, T)>> get_nand_primitives() {
  std::vector<std::function<T(T, T)>> primitives;
  primitives.emplace_back(
      std::bind(&Nand_1<T>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&Nand_1<T>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&Nand_1<T>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&Nand_1<T>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&Nand_1<T>, std::placeholders::_1, std::placeholders::_2));
  return primitives;
}

template <typename T>
std::vector<std::function<T(T)>> get_not_primitives() {
  std::vector<std::function<T(T)>> primitives;
  primitives.emplace_back(std::bind(&NotR<T, 0>, std::placeholders::_1));
  primitives.emplace_back(std::bind(&NotR<T, 1>, std::placeholders::_1));
  primitives.emplace_back(std::bind(&NotR<T, 2>, std::placeholders::_1));
  primitives.emplace_back(std::bind(&NotR<T, 3>, std::placeholders::_1));
  primitives.emplace_back(std::bind(&NotR<T, 4>, std::placeholders::_1));
  primitives.emplace_back(std::bind(&NotR<T, 5>, std::placeholders::_1));
  primitives.emplace_back(std::bind(&NotR<T, 6>, std::placeholders::_1));
  primitives.emplace_back(std::bind(&NotR<T, 7>, std::placeholders::_1));
  primitives.emplace_back(std::bind(&NotR<T, 8>, std::placeholders::_1));
  primitives.emplace_back(std::bind(&NotR<T, 9>, std::placeholders::_1));
  return primitives;
}

template <typename T>
std::vector<std::function<T(T, T)>> get_and_primitives() {
  std::vector<std::function<T(T, T)>> primitives;
  primitives.emplace_back(
      std::bind(&AndR<T, 0>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&AndR<T, 1>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&AndR<T, 2>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&AndR<T, 3>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&AndR<T, 4>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&AndR<T, 5>, std::placeholders::_1, std::placeholders::_2));
  return primitives;
}

template <typename T>
std::vector<std::function<T(T, T)>> get_or_primitives() {
  std::vector<std::function<T(T, T)>> primitives;
  primitives.emplace_back(
      std::bind(&OrR<T, 0>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&OrR<T, 1>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&OrR<T, 2>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&OrR<T, 3>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&OrR<T, 4>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&OrR<T, 5>, std::placeholders::_1, std::placeholders::_2));
  return primitives;
}

template <typename T>
std::vector<std::function<T(T, T)>> get_xor_primitives() {
  std::vector<std::function<T(T, T)>> primitives;
  primitives.emplace_back(
      std::bind(&XorR<T, 0>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&XorR<T, 1>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&XorR<T, 2>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&XorR<T, 3>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&XorR<T, 4>, std::placeholders::_1, std::placeholders::_2));
  primitives.emplace_back(
      std::bind(&XorR<T, 5>, std::placeholders::_1, std::placeholders::_2));
  return primitives;
}

template <typename T>
inline void check_math(T value) {
  auto nor_primitives = get_nor_primitives<T>();
  auto nand_primitives = get_nand_primitives<T>();
  auto not_primitives = get_not_primitives<T>();
  auto and_primitives = get_and_primitives<T>();
  auto or_primitives = get_or_primitives<T>();
  auto xor_primitives = get_xor_primitives<T>();
  for (T i = 0; i <= 64; i++) {
    for (auto& f : nor_primitives) {
      ASSERT_EQ(execute_operation<T>(nor_, value, i), f(value, i));
    }

    for (auto& f : nand_primitives) {
      ASSERT_EQ(execute_operation<T>(nand_, value, i), f(value, i));
    }

    for (auto& f : not_primitives) {
      ASSERT_EQ(execute_operation<T>(not_, value, i), f(value));
    }

    for (auto& f : and_primitives) {
      ASSERT_EQ(execute_operation<T>(and_, value, i), f(value, i));
    }

    for (auto& f : or_primitives) {
      ASSERT_EQ(execute_operation<T>(or_, value, i), f(value, i));
    }

    for (auto& f : xor_primitives) {
      ASSERT_EQ(execute_operation<T>(xor_, value, i), f(value, i));
    }
  }

  for (T i = 0; i <= 64; i++) {
    for (T j = 0; j <= 64; j++) {
      for (auto& f : nor_primitives) {
        ASSERT_EQ(execute_operation<T>(nor_, j, i), f(j, i));
      }

      for (auto& f : nand_primitives) {
        ASSERT_EQ(execute_operation<T>(nand_, j, i), f(j, i));
      }

      for (auto& f : not_primitives) {
        ASSERT_EQ(execute_operation<T>(not_, j, i), f(j));
      }

      for (auto& f : and_primitives) {
        ASSERT_EQ(execute_operation<T>(and_, j, i), f(j, i));
      }

      for (auto& f : or_primitives) {
        ASSERT_EQ(execute_operation<T>(or_, j, i), f(j, i));
      }

      for (auto& f : xor_primitives) {
        ASSERT_EQ(execute_operation<T>(xor_, j, i), f(j, i));
      }
    }
  }
}

TEST(MathTests, RANGE_MAX_VALUES_AND_LOOP) {
  std::apply(
      [](auto&... value) {
        (..., check_math<std::remove_reference_t<decltype(value)>>(value));
      },
      unsigned_max);

  std::apply(
      [](auto&... value) {
        (..., check_math<std::remove_reference_t<decltype(value)>>(value));
      },
      signed_min);
}

TEST(CompileTimeRandom, CHECK_COMPILE_TIME_RANDOM_VALIDITY) {
  auto x = RND;
  auto y = RND;
  EXPECT_NE(x, y);
  x = RND;
  y = RND;
  EXPECT_NE(x, y);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}