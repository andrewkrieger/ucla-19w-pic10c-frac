# Example code: C++ maps with custom comparison and hash functions

This repository shows how to instantiate `std::map` and `std::unordered_map` in
C++, both with a default comparison function, a custom comparison function, and
custom hash functions. It also compares different hash functions, including both
a pretty bad hash function and two reasonable hash functions.

The file [boost_hash_combine.h](boost_hash_combine.h) is an adaptation of
[Boost's](https://www.boost.org) `hash_combine` function: given an existing
`size_t seed` and a new `size_t value`, it mixes `value` into the `seed` hash. I
adapted it for independence from the rest of Boost, to reduce dependencies for
this example.

`fraction.cpp` and `fraction.h` define the key type for the maps we'll create.
They represent fractions that are generally *not* in reduced terms, so
`operator==`, `operator<`, and related operations have to do non-trivial
comparisons (e.g. cross-multiply).

`main.cpp` creates 5 different maps:

- Two instances of `std::map`. One uses `operator<` from the `Fraction` class.
  The other uses a custom comparison function **which is inconsistent with
  `operator==`**. The second `std::map` is larger than the other 4, since
  fractions like `1/2` and `3/6` are considered distinct.

- Three instances of `std::unordered_map`, each with a different hash function.
  The first is just `numerator ^ denominator`, which performs fairly badly. The
  other two are reasonably good; one is hand-written, and the other just calls
  Boost's `hash_combine`.

The three source files I wrote (`fraction.h`, `fraction.cpp`, and `main.cpp`)
are released under the
[CC0](https://creativecommons.org/share-your-work/public-domain/cc0/) public
domain dedication. `boost_hash_combine.h` is an adaptation of `hash.hpp` from
Boost, which is licensed under the
[Boost Software License](http://www.boost.org/LICENSE_1_0.txt), included as
`BOOST_LICENSE_1_0.txt` in this project.
