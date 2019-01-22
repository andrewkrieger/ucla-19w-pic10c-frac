// vim: tw=60
#include "fraction.h"

#include <random>

#include <iostream>

// anonymous (unnamed) namespace: functions in here can only
// be used in this .cpp file.
namespace {
// https://en.wikipedia.org/wiki/Greatest_common_divisor#Using_Euclid's_algorithm
int gcd(int a, int b) {
  // std::cerr << "gcd(" << a << ", " << b << ")\n";
  if (a < 0) a *= -1;
  if (b < 0) b *= -1;
  if (a < b) std::swap(a, b);
  while (b != 0) {
    int new_a = b;
    int new_b = a % b;
    a = new_a;
    b = new_b;
    // std::cerr << "  gcd(" << a << ", " << b << ")\n";
  }
  return a;
}
}  // namespace

bool Fraction::operator==(const Fraction& rhs) const {
  // cast to long to reduce the risk of overflows
  return ((long)num) * rhs.den == ((long)den) * rhs.num;
}

bool Fraction::operator!=(const Fraction& rhs) const {
  return !(*this == rhs);
}

std::ostream& operator<<(std::ostream& os,
                         const Fraction& f) {
  return os << f.num << '/' << f.den;
}

Fraction random_fraction() {
  // C++11 random numbers are complicated...
  static int min = -1000, max = 1000;
  static std::mt19937 gen;
  static std::uniform_int_distribution<int> dist(min, max);
  Fraction f;
  f.num = dist(gen);
  do {
    f.den = dist(gen);
  } while (f.den == 0);
  return f;
}

Fraction reduce_fraction(Fraction f) {
  int div = gcd(f.num, f.den);
  if (f.den < 0) div *= -1;
  Fraction reduced = f;
  reduced.num /= div;
  reduced.den /= div;
  return reduced;
}
