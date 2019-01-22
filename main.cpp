// vim: tw=60
#define _POSIX_C_SOURCE 200809l

#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <unordered_map>

#include "fraction.h"

// Define a custom comparison function for class Fraction.
// This is a lexicographical comparison. It looks at num
// first, and if num is different, returns the comparison
// result. Only if num is equal, it looks at den. Two
// fractions are only considered equal if both num and den
// are equal.
//
// Obviously, this isn't "correct" for fractions. For
// example, this will say that (1/2) < (3/6) because 1 < 3.
//
// C++ boilerplate: we define a new struct (or class) with
// operator(). That operator takes two arguments of type
// Fraction and returns true if the first is strictly less
// than the second.
struct fraction_lexi_comp {
  bool operator()(const Fraction& a,
                  const Fraction& b) const {
    // This is very mechanical-looking code. In C++20, it
    // can be auto-generated (I think).
    if (a.num < b.num) return true;
    if (a.num > b.num) return false;
    if (a.den < b.den) return true;
    if (a.den > b.den) return false;
    return false;
  }
};

// Define a custom hash function for Fraction. This uses
// XOR, which is commonly recommended in old online sources
// but is actually a pretty bad way to combine hash values.
// It causes lots of collisions.
struct fraction_bad_hash {
  size_t operator()(const Fraction& a) const {
    Fraction b = reduce_fraction(a);
    return b.num ^ b.den;  // bitwise XOR
  }
};

// Adds fractions of the form a/b into the map, where a and
// b both run from -N up to N.
template <typename M>
float add_to_map(M& map, int N) {
  // Weird c++11 timing code; feel free to ignore.
  auto start = std::chrono::system_clock::now();
  for (int num = -N; num <= N; num++) {
    for (int den = -N; den <= N; den++) {
      if (den == 0) continue;
      Fraction a(num, den);
      Fraction b = reduce_fraction(a);
      map[a] = b;
    }
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<float> dur = end - start;
  return dur.count();
}

int main() {
  int N = 400;

  std::cout << "*** Using std::map ***\n\n";

  // A std::map, using default operator<.
  std::map<Fraction, Fraction> m1;
  float t1 = add_to_map(m1, N);
  std::cout << "m1: Took " << t1 << " seconds to add "
            << m1.size() << " elements." << std::endl;

  // A std::map, using our custom comparison function.
  std::map<Fraction, Fraction, fraction_lexi_comp> m2;
  float t2 = add_to_map(m2, N);
  std::cout << "m2: Took " << t2 << " seconds to add "
            << m2.size() << " elements." << std::endl;

  std::cout << "*** Using std::unordered_map ***\n"
            << std::endl;

  std::unordered_map<Fraction, Fraction, fraction_bad_hash>
      m3;
  float t3 = add_to_map(m3, N);
  std::cout << "m3: Took " << t3 << " seconds to add "
            << m3.size() << " elements." << std::endl;

  return 0;
}
