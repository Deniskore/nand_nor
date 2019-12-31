#include <iostream>
#include <random>
#include <vector>
#include <chrono>

#include "compile_random.h"
#include "nand_nor.h"
#include "nn_integer.h"

enum ECommand : uint64_t {
  nand_ = 1,
  nor_ = 2,
  and_ = 3,
  or_ = 4,
  xor_ = 5,
};

void add_commands(std::vector<uint64_t>& code) {
  const std::vector<uint64_t> ops = {nand_, nor_, and_, or_, xor_};
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<uint64_t> uid(0, 4);
  std::uniform_int_distribution<uint64_t> uid_32(1, UINT32_MAX);
  code.emplace_back(ops[uid(rng)]);
  code.emplace_back(uid_32(rng));
  code.emplace_back(uid_32(rng));
  code.emplace_back(ops[uid(rng)]);
  code.emplace_back(uid_32(rng));
  code.emplace_back(uid_32(rng));
}

uint64_t diff_me(uint64_t x) {
  using u64 = uint64_t;
  CNNInt<u64> r(std::chrono::system_clock::now().time_since_epoch().count());
  CNNInt<u64> test(x);
  test++;
  test--;
  test|=r;
  test^=r;
  test&=r;
  test^=r;
  test = ~test;
  return test.value();
}

uint64_t execute_code(std::vector<uint64_t>& code) {
  using u64 = uint64_t;
  CNNInt<u64> ip(0);
  auto extract_two_operands = [&]() {
    auto a = code[ip.value()];
    auto b = code[(++ip).value()];
    ip++;
    return std::make_pair(a, b);
  };
  u64 result = 0;

  while (ip.value() <= code.size() - 1) {
    auto command = code[ip.value()];
    ip++;
    auto ops = extract_two_operands();

    switch (command) {
      case nand_: {
        result = XorR<u64, RND>(NandR<u64, RND>(ops.first, ops.second), result);
        break;
      }
      case nor_: {
        result = XorR<u64, RND>(NorR<u64, RND>(ops.first, ops.second), result);
        break;
      }
      case and_: {
        result = XorR<u64, RND>(AndR<u64, RND>(ops.first, ops.second), result);
        break;
      }
      case or_: {
        result = XorR<u64, RND>(OrR<u64, RND>(ops.first, ops.second), result);
        break;
      }
      case xor_: {
        result = XorR<u64, RND>(XorR<u64, RND>(ops.first, ops.second), result);
        break;
      }
      default:
        break;
    }
  }
  return result;
}

int main() {
  std::vector<uint64_t> code;
  add_commands(code);
  auto now = std::chrono::system_clock::now().time_since_epoch().count();
  std::cout << execute_code(code) << std::endl;
  std::cout << diff_me(now) << std::endl;
  return 0;
}
