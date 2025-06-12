# `fsstream` library
`fsstream` is a header-only string stream library, it provides standard-like stream to translate various values to characters, and you can specialize the `parser` and `scanner` classes to support your custom types.

You can switch from standard string stream to `fsstream` by swapping the namespace from `std` to `fast_sstream`.

This library requires `C++17` of later.

## Include
```cpp
#include "fsstream.hpp"
// Don't do this, this will pollute your namespace with too many empty structures and alias.
using namespace fast_sstream;
//Recommend:
using fast_sstream::stringstream;
```

## Feature

* Faster than `sstream`.
* Can output directly to standard stream(e.g., `stdout`,`std::basic_ostream`).
* Standard-like API.

## Usage

```cpp
fast_sstream::stringstream ss;
//insert
ss<<"Hello world\n"<<39<<true<<0.01<<'\n';
//output
std::cout<<ss.view()<<ss.str();
ss>>std::cout>>ss>>stdout;
```
output:
```
Hello world
39true0.010000
Hello world
39true0.010000
Hello world
39true0.010000
Hello world
39true0.010000
```

## Limitation

* not thread-safe.
* no `std::locale` support.
* Natively supports `char` only.
