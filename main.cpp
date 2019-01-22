// vim: tw=60
#define _POSIX_C_SOURCE 200809l

#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <unordered_map>

#include "fraction.h"

struct fraction_lexi_comp {
  bool operator()(const Fraction& a, const Fraction& b) {
    if (a.num < b.num) return true;
    if (a.num > b.num) return false;
    if (a.den < b.den) return true;
    if (a.den > b.den) return false;
    return false;
  }
};

struct fraction_bad_hash {
  size_t operator()(const Fraction& a) const {
    Fraction b = reduce_fraction(a);
    return b.num + b.den;
  }
};

struct fraction_less_bad_hash {
  size_t operator()(const Fraction& a) const {
    Fraction b = reduce_fraction(a);
    size_t comb = (((size_t)b.num) << 8) ^ ((size_t)b.den);
    size_t base =
        comb ^ (comb << 16) ^ (comb << 32) ^ (comb << 48);
    // Multiply by a random odd number for further mixing.
    return base * 0x9fd60a67299c6933ULL;
  }
};

template <typename M>
float make_frac_map(M& m, int N) {
  auto start = std::chrono::system_clock::now();
  for (int num = -N; num <= N; num++) {
    for (int den = -N; den <= N; den++) {
      if (den == 0) continue;
      Fraction a(num, den);
      Fraction b = reduce_fraction(a);
      m[a] = b;
    }
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<float> dur = end - start;
  return dur.count();
}

int main() {
  int N = 10;

  std::map<Fraction, Fraction> m1;
  float t1 = make_frac_map(m1, N);
  std::cout << "m1 took " << t1 << " seconds and has "
            << m1.size() << " elements." << std::endl;
  for (const auto& p : m1) {
    std::cout << p.first << "\t=>\t" << p.second << '\n';
  }

  std::map<Fraction, Fraction, fraction_lexi_comp> m2;
  float t2 = make_frac_map(m2, N);
  std::cout << "m2 took " << t2 << " seconds and has "
            << m2.size() << " elements." << std::endl;

  std::unordered_map<Fraction, Fraction, fraction_bad_hash>
      m3;
  float t3 = make_frac_map(m3, N);
  std::cout << "m3 took " << t3 << " seconds and has "
            << m3.size() << " elements." << std::endl;

  std::unordered_map<Fraction, Fraction,
                     fraction_less_bad_hash>
      m4;
  float t4 = make_frac_map(m4, N);
  std::cout << "m4 took " << t4 << " seconds and has "
            << m4.size() << " elements." << std::endl;

  return 0;
}
