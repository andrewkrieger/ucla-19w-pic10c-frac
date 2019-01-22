// vim: tw=60
#include <ostream>

struct Fraction {
 public:
  int num, den;

  bool operator==(const Fraction& rhs) const;
  bool operator!=(const Fraction& rhs) const;
};

std::ostream& operator<<(std::ostream& os,
                         const Fraction& f);

Fraction random_fraction();
Fraction reduce_fraction(Fraction f);