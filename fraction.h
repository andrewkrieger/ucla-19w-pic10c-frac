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

#include <ostream>

// A simple fraction type, with only the operators defined that are interesting
// for our purposes (storing in a map).
struct Fraction {
 public:
  int num, den;
  Fraction(int num = 0, int den = 1) : num(num), den(den) {}

  bool operator==(const Fraction& rhs) const;
  bool operator<(const Fraction& rhs) const;
};

std::ostream& operator<<(std::ostream& os,
                         const Fraction& f);

std::string fraction_to_string(const Fraction& f);

// Not actually used, but kept for reference.
Fraction random_fraction();

// This is used heavily in the example!
Fraction reduce_fraction(const Fraction& f);
