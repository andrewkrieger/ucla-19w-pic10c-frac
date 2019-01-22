// Copyright Andrew Krieger, 2019
// A simplified version of boost::hash_combine, taken from
// <https://github.com/boostorg/container_hash/blob/f054fe932f4d5173bfd6dad5bcff5738a7aff0be/include/boost/container_hash/hash.hpp>
// Changes:
//   - boost::hash_detail::hash_combine_impl<size_t> renamed to
//     boost::hash_combine, to avoid extra template parameters (at the risk of
//     breaking terribly on 32-bit machines).
//   - Minor changes to variable names and definition of variable `m`.
//   - License renamed to BOOST_LICENCE_1_0.txt.
//
// Original copyright:
//   Copyright 2005-2014 Daniel James.
//   Distributed under the Boost Software License, Version 1.0. (See
//   accompanying file LICENSE_1_0.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)
//
//   Based on Peter Dimov's proposal
//   http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2005/n1756.pdf
//   issue 6.18./

#include <cstdlib>

// boost is a common C++ library. Among (many!) other things, it provides a
// hash_combine() function, made for combining hash codes of several input
// objects. I copied the implementation into the function below. Note that this
// will probably fail horribly if you use a 32-bit compiler.
void boost_hash_combine(size_t& seed, size_t value) {
  constexpr size_t m = 0xc6a4a7935bd1e99ULL;
  constexpr int r = 47;

  value *= m;
  value ^= value >> r;
  value *= m;

  seed ^= value;
  seed *= m;

  // Completely arbitrary number, to prevent 0's
  // from hashing to 0.
  seed += 0xe6546b64;
}
