// vim: tw=60
#define _POSIX_C_SOURCE 200809l

#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <unordered_map>

#include "boost_hash_combine.h"
#include "fraction.h"

// A lexicographical comparison function. To compare two
// fractions, it first compares their numerators. If the
// numerators are unequal, so are the fractions. Then it
// compares denominators.
//
// Lexicographical ordering is a good way to define an order
// relation for things like an unordered pair, where every
// pair of values is allowed, and no pairs are equal unless
// they're identical. It's not the correct definition of <
// for fractions, since e.g. (1/2) < (3/6) in
// lexicographical order (because 1 < 3). That's why m2 has
// "too many" keys: fractions like (1/2) and (3/6) are
// considered distinct map keys under this ordering.
struct fraction_lexi_comp {
  bool operator()(const Fraction& a, const Fraction& b) {
    // This code is very mechanical, but still necessary to
    // write as of C++17. I think C++20 might have some
    // ability to automate lexicographical comparisons, but
    // I haven't checked recently.
    if (a.num < b.num) return true;
    if (a.num > b.num) return false;
    if (a.den < b.den) return true;
    if (a.den > b.den) return false;
    return false;
  }
};

// A common but not-great way of hashing a pair of objects
// is to XOR together their hash codes. This does combine
// the hash codes, but poorly. For example, this hash
// function returns 0 for any fraction of the form (A/A),
// since A ^ A == 0.
struct fraction_bad_hash {
  size_t operator()(const Fraction& a) const {
    // First reduce to a canonical representation (meaning:
    // if two Fraction objects are equal, then their
    // reduce_fraction forms are identical). This is
    // important! If we skip this step, then probably
    // hash(1/2) != hash(5/10), which violates the
    // requirements for a hash code. You'll get weird
    // results in the map.
    Fraction b = reduce_fraction(a);
    return b.num ^ b.den;
  }
};

// A better hash function (I haven't tested it in detail,
// but in my limited testing it's much better). When
// combining hash codes, your goal is that any slight
// difference in either field will massively affect the
// resulting hash code. That's not true for the XOR method
// above (changing one bit in num will only change one bit
// in the XOR'ed result), plus as mentioned above XOR leads
// to lots of collisions.
struct fraction_less_bad_hash {
  size_t operator()(const Fraction& a) const {
    // Need to reduce to a canonical form (see above).
    Fraction b = reduce_fraction(a);
    // Combine num and den into one object. There are
    // probably better ways of combining two hash codes, but
    // to defend this one: in my examples, num and den are
    // usually about 8 bits, and they take pretty
    // independent values. So, this code basically turns two
    // 8 bits ints into one 16 bit int.
    size_t comb = (((size_t)b.num) << 8) ^ ((size_t)b.den);
    // Make sure that all 64 bits of the result are
    // affected. Again, there are probably better ways of
    // accomplishing this.
    size_t base =
        comb ^ (comb << 16) ^ (comb << 32) ^ (comb << 48);
    // Multiply by a random odd number for further mixing.
    // It's a good idea to choose an odd number. Multiplying
    // by an odd number will map all 2**64 size_t values
    // into 2**64 different values (it's an invertible,
    // 1-to-1 function). If the constant were even, then the
    // product must always be even; we'd be mapping 2**64
    // size_t values into 2**63 **even** values, so we'd be
    // losing bits.
    return base * 0x9fd60a67299c6933ULL;
  }
};

// Really, using boost::hash_combine (which will hopefully
// make it into the standard library eventually) is the
// easiest way to have a pretty-good hash function.
struct fraction_boost_hash {
  size_t operator()(const Fraction& a) const {
    Fraction b = reduce_fraction(a);
    size_t seed = 0;
    boost_hash_combine(seed, b.num);
    boost_hash_combine(seed, b.den);
    return seed;
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

// Compute the RMS (root-mean-square) bucket size, i.e.
//     sqrt( sum( size(b)**2 for b in m.buckets() ) )
// This is a kind of average bucket size, but it assigns
// more weight to large buckets. For example:
//   - if the map contains 1,000,000 elements all in one
//     bucket, the result is:
//         sqrt(1,000,000**2) = 1,000,000
//              ^ only one bucket to sum over
//   - if there are 1,000 elements per bucket spread among
//     1,000 buckets, the result is:
//     sqrt(1000     *     (1000**2)) = 31622.78.
//          ^ # buckets     ^ bucket size, squared
template <typename M>
double rms_bucket_size(const M& map) {
  double res;
  for (size_t i = 0; i < map.bucket_count(); i++) {
    double x = map.bucket_size(i);
    res += x * x;
  }
  return std::sqrt(res);
}

int main() {
  int N = 400;

  std::cout << "*** using std::map ***" << std::endl;

  // Use the natural ordering on fractions (i.e. operator<).
  std::map<Fraction, Fraction> m1;
  float t1 = make_frac_map(m1, N);
  std::cout << "m1 took " << t1 << " seconds and has "
            << m1.size() << " elements." << std::endl;

  // Use a custom ordering function (here, lexicographical
  // order). Note that this map has distinct keys that are
  // == to each other, e.g. (1/2) and (3/6); see comment
  // near struct fraction_lexi_comp above).
  std::map<Fraction, Fraction, fraction_lexi_comp> m2;
  float t2 = make_frac_map(m2, N);
  std::cout << "m2 took " << t2 << " seconds and has "
            << m2.size() << " elements." << std::endl;

  size_t num_unique_fracs = m1.size();
  std::cout << "\n\n*** using std::unordered_map ***\n"
            << "rms(bucket_size) measures the number of\n"
            << "hash collisions. It's a number between\n"
            << std::sqrt(num_unique_fracs) << " and "
            << num_unique_fracs << "; smaller is better!\n"
            << std::endl;

  // Use a custom hash function. This one is pretty bad, and
  // building m3 takes much longer than the other maps.
  std::unordered_map<Fraction, Fraction, fraction_bad_hash>
      m3;
  float t3 = make_frac_map(m3, N);
  std::cout << "m3 took " << t3 << " seconds and has "
            << m3.size() << " elements\n    in "
            << m3.bucket_count() << " buckets, "
            << "rms(bucket_size) = " << rms_bucket_size(m3)
            << std::endl;

  // Use a much improved, hand-written hash function.
  std::unordered_map<Fraction, Fraction,
                     fraction_less_bad_hash>
      m4;
  float t4 = make_frac_map(m4, N);
  std::cout << "m4 took " << t4 << " seconds and has "
            << m4.size() << " elements\n    in "
            << m4.bucket_count() << " buckets, "
            << "rms(bucket_size) = " << rms_bucket_size(m4)
            << std::endl;

  // Use a hash function based on boost's hash_combine. This
  // is what you should typically default to if you need to
  // define a hash function.
  std::unordered_map<Fraction, Fraction,
                     fraction_less_bad_hash>
      m5;
  float t5 = make_frac_map(m5, N);
  std::cout << "m5 took " << t5 << " seconds and has "
            << m5.size() << " elements\n    in "
            << m5.bucket_count() << " buckets, "
            << "rms(bucket_size) = " << rms_bucket_size(m5)
            << std::endl;

  return 0;
}
