// vim: tw=60
#define _POSIX_C_SOURCE 200809l

#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <unordered_map>

#include "fraction.h"

int main() {
  std::map<Fraction, Fraction> reduced;
  constexpr int N = 1000;
  auto start = std::chrono::system_clock::now();
  for (int num = -N; num <= N; num++) {
    for (int den = -N; den <= N; den++) {
      if (den == 0) continue;
      Fraction a(num, den);
      Fraction b = reduce_fraction(a);
      reduced[a] = b;
    }
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<float> dur = end - start;
  std::cout << "run time (seconds) = " << dur.count()
            << '\n';
  std::cout << "reduced.size() = " << reduced.size()
            << '\n';
  return 0;
}
