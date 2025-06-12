# `fsstream` library
`fsstream` is a header-only string stream library, it provided standard like stream to trainslate various value to character, and you can add specialization of `parser` and `scanner` class to support your custom type.

You can switch from standard to `fsstream` just swap the namespace `std` by `fast_sstream`(but it hasn't support `wchar_t` yet)

This library need `C++17` at least.

## Include
```cpp
#include "fsstream.hpp"
using namespace fast_sstream; // Don't do this, it has too many empty struct and will pollute your namespace
using fast_sstream::stringstream; // Use this instead
```

## Feature

* faster than `sstream`
* can direct output to standard stream(`stdout`,`std::basic_ostream`)

## Usage

```cpp
fast_sstream::stringstream ss;
//insert
ss<<"Hello world\n"<<39<<true<<0.01<<'\n';
//output
std::cout<<ss.view()<<ss.str();
ss>>std::cout;
ss>>stdout;
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
