// frac - Sample code for working with maps in C++.
// Written in 2019 by Andrew Krieger <akrieger@math.ucla.edu>
//
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide.  This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along
// with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "fraction.h"

#include <random>
#include <sstream>

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

bool Fraction::operator<(const Fraction& rhs) const {
  // From high school (?) algebra: we know that
  //     a/b < c/d   <=>   a*d < b*c
  // if b and d are both positive. If b or d is negative, we need to reverse the
  // inequality (because we multiplied both sides by a negative number). Use
  // variable `reverse` to keep track of whether the inequality is reversed.
  bool reverse = false;
  if (den < 0) reverse = !reverse;
  if (rhs.den < 0) reverse = !reverse;
  if (reverse) {
    return ((long)num) * rhs.den > ((long)den) * rhs.num;
  } else {
    return ((long)num) * rhs.den < ((long)den) * rhs.num;
  }
}

std::ostream& operator<<(std::ostream& os,
                         const Fraction& f) {
  return os << f.num << '/' << f.den;
}

std::string fraction_to_string(const Fraction& f) {
  std::ostringstream oss;
  oss << f;
  return oss.str();
}

Fraction random_fraction() {
  // C++11 random numbers are complicated...
  static int min = -1000, max = 1000;
  // A random bit generator. This supplies randomness.
  static std::mt19937 gen;
  // A random distribution (in this case: uniform distribution over ints from
  // min to max). This takes in randomness from the RNG object above, and uses
  // that randomness to produce nice random ints.
  static std::uniform_int_distribution<int> dist(min, max);
  Fraction f;
  f.num = dist(gen);  // Get a random int.
  do {
    f.den = dist(gen);
  } while (f.den == 0);  // Ensure den != 0.
  return f;
}

Fraction reduce_fraction(const Fraction& f) {
  int div = gcd(f.num, f.den);
  if (f.den < 0) div *= -1;
  Fraction reduced = f;
  reduced.num /= div;
  reduced.den /= div;
  return reduced;
}
