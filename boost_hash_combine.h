// Copyright Andrew Krieger, 2019
// A simplified version of boost::hash_combine, taken from
// <https://github.com/boostorg/container_hash/blob/f054fe932f4d5173bfd6dad5bcff5738a7aff0be/include/boost/container_hash/hash.hpp>
// Changes:
//   - Functions boost::hash_detail::hash_combine_impl<uint32_t|uint64_t>
//     copied into this file; other functions deleted.
//   - boost_hash_combine defined, to call hash_combine_impl.
//   - Code reformatted; namespaces renamed.
//   - License downloaded and renamed to BOOST_LICENCE_1_0.txt.
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

#include <cstddef>
#include <cstdint>

#define BOOST_FUNCTIONAL_HASH_ROTL32(x, r) (x << r) | (x >> (32 - r))

namespace {

inline void hash_combine_impl(std::uint32_t& h1, std::uint32_t k1) {
  const std::uint32_t c1 = 0xcc9e2d51;
  const std::uint32_t c2 = 0x1b873593;

  k1 *= c1;
  k1 = BOOST_FUNCTIONAL_HASH_ROTL32(k1, 15);
  k1 *= c2;

  h1 ^= k1;
  h1 = BOOST_FUNCTIONAL_HASH_ROTL32(h1, 13);
  h1 = h1 * 5 + 0xe6546b64;
}

inline void hash_combine_impl(std::uint64_t& h, std::uint64_t k) {
  constexpr std::uint64_t m = 0xc6a4a7935bd1e99ULL;
  constexpr int r = 47;

  k *= m;
  k ^= k >> r;
  k *= m;

  h ^= k;
  h *= m;

  // Completely arbitrary number, to prevent 0's
  // from hashing to 0.
  h += 0xe6546b64;
}

}  // namespace

void boost_hash_combine(std::size_t& seed, std::size_t value) {
  hash_combine_impl(seed, value);
}
