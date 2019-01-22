#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <unordered_map>

#include "fraction.h"

int main() {
  Fraction a, b, c, d;
  a = random_fraction();
  std::cout << "a = " << a << std::endl;

  b = reduce_fraction(a);
  std::cout << "b = " << b << std::endl;

  c.num = 1000;
  c.den = -100;
  std::cout << "c = " << c << std::endl;

  d = reduce_fraction(c);
  std::cout << "d = " << d << std::endl;
  std::cout << std::endl;

  std::cout << "a == b: " << (a == b ? "true" : "false") << std::endl;
  std::cout << "a == c: " << (a == c ? "true" : "false") << std::endl;
  std::cout << "a == d: " << (a == d ? "true" : "false") << std::endl;

  std::cout << "b == c: " << (b == c ? "true" : "false") << std::endl;
  std::cout << "b == d: " << (b == d ? "true" : "false") << std::endl;

  std::cout << "c == d: " << (c == d ? "true" : "false") << std::endl;
}
