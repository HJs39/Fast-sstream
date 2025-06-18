# `fsstream` library

[中文版本](README_ZH.md)

`fsstream` is a header-only string stream library, it provides standard-like string stream for converting between various data types and character sequences. You can specialize the `parser` and `scanner` classes to support your custom types.

You can switch from standard string stream to `fsstream` by swapping the namespace from `std` to `fast_sstream`.

## Limitation

* requires `C++17` or later.
* not thread-safe.
* no `std::locale` support.
* Natively supports `char` only.

## Feature

* Faster than standard `sstream`.
* Can output directly to standard stream(e.g., `stdout`,`std::basic_ostream`).
* Standard-like API.

## Usage

### Include

```cpp
#include "fsstream.hpp"

using namespace fast_sstream;
//^Don't do this, it will pollute your namespace with too many empty structures and aliases

//Recommended:
using fast_sstream::stringstream;
using fast_sstream::istringstream;
using fast_sstream::ostringstream;
//or
namespace fss = fast_sstream;
```

### Basic using

You can construct an empty stream and use it like the one provided by standard.(Note: When constructing with a string, `fsstream` takes it by reference instead of making a copy.)

```cpp
fast_sstream::stringstream ss;
ss << "Hello world " << 39 << ' ' << true << ' ' << 0.01 << '\n';
std::cout << ss.view();//or ss.str()
```

output:

```text
Hello world 39 true 0.010000
```

You can use the manipulators provided by `fast_sstream` to modify the stream's state, similar to the standard library.

```cpp
fast_sstream::stringstream ss;
ss << "Before setting:" << "\ninteger:" << 39 << "\nboolean:" << true << "\nfloating point:" << 1.732;
ss << fast_sstream::noboolalpha << fast_sstream::setprecision(5) << fast_sstream::hex;
ss<<"\nAfter setting:"<< "\ninteger:" << 39 << "\nboolean:" << true << "\nfloating point:" << 1.732;
std::cout << ss.view();
```

output:

```text
Before setting:
integer:39
boolean:true
floating point:1.732
After setting:
integer:39
boolean:1
floating point:1.bb646p+0
```

To output buffer data, you can use `operator>>` specialized for standard stream.

```cpp
fast_sstream::stringstream ss;
ss << "output test\n";
ss >> stdout >> std::cout;
```

output:

```text
output test
output test
```

Use the `fast_sstream::nostdcpy` manipulator to consume the buffer's data instead of copying it.

```cpp
fast_sstream::stringstream ss;
ss << "output test\n" << fast_sstream::nostdcpy;
ss >> stdout;
ss << 3939;
ss >> std::cout;
```

output:

```text
output test
3939
```

You can support custom types by adding specializations of `parser` and `scanner` to the `fast_sstream` namespace.

```cpp
struct My_type {
    int i;
    float f;
    bool b;
};

namespace fast_sstream {
    template<typename T>
    struct parser<T, char, std::enable_if_t<same<T,My_type>, void>> {
        template<typename traits,typename Buf>
        static void parse(T&& value, traits* trait, Buf* buf) {
            //            ^First: the value to be parsed
            //             Second: a pointer to the I/O traits class that called parse(). You can access its members, as parser is a friend.(not recommended)
            //             Third: a pointer to the underlying buffer held by the traits. You can also access its members, but this is not recommended.
            buf->sputc('[');
            fast_sstream::parser<int, char>::parse(std::forward<int>(value.i), trait, buf);
            buf->sputc(',');
            fast_sstream::parser<float, char>::parse(std::forward<float>(value.f), trait, buf);
            buf->sputc(',');
            fast_sstream::parser<bool, char>::parse(std::forward<bool>(value.b), trait, buf);
            buf->sputc(']');
        };
    };
    template<typename T>
    struct scanner<T, char, std::enable_if_t<same<T, My_type>, void>> {
        template<typename traits,typename Buf>
        static void scan(T& value, traits* trait, Buf* buf) {
            //           ^ Parameters are similar to parser::parse, but the first argument (value) is an out-parameter to store the scanned result.
            buf->gbump(1);
            fast_sstream::scanner<int, char>::scan(value.i, trait, buf);
            buf->gbump(1);
            fast_sstream::scanner<float, char>::scan(value.f, trait, buf);
            buf->gbump(1);
            fast_sstream::scanner<bool, char>::scan(value.b, trait, buf);
            buf->gbump(1);
            // This is just a simple example. You can add more checks and throw exceptions to ensure the result is correct.
        };
    };
}

int main() {
    fast_sstream::stringstream ss;
    My_type value{ 39,0.555,false }, dest{};
    ss << value;
    ss >> stdout >> dest;
    std::cout << std::boolalpha << "\nscan result:[" << dest.i << ',' << dest.f << ',' << dest.b << "]\n";
    return 0;
}
```

output:

```text
[39,0.555,false]
scan result:[39,0.555,false]
```

### format

`format` is a simple formatting function available on any stream derived from `output_traits` or `io_traits`. It provides a simple way to format strings as an alternative to chained `operator<<` calls.

Below is `fast-sstream`'s formatting syntax:

* The `%` character is a placeholder for a formatted value. When `format` scans this character, it will take the next argument from the parameter pack and call `parser<TYPE>::parse` to format it.

* The `^` character is an ignore placeholder. When `format` scans this character, it will consume the next argument from the parameter pack without formatting it.

* The `\` character is an escape character. It causes `format` to ignore any special meaning of the character that follows it, writing it directly to the buffer instead.

You can use `format` to simplify coding of `parser`.

Example from above:

```cpp
namespace fast_sstream {
    //old
    template<typename T>
    struct parser<T, char, std::enable_if_t<same<T,My_type>, void>> {
        template<typename traits,typename Buf>
        static void parse(T&& value, traits* trait, Buf* buf) {
            buf->sputc('[');
            fast_sstream::parser<int, char>::parse(std::forward<int>(value.i), trait, buf);
            buf->sputc(',');
            fast_sstream::parser<float, char>::parse(std::forward<float>(value.f), trait, buf);
            buf->sputc(',');
            fast_sstream::parser<bool, char>::parse(std::forward<bool>(value.b), trait, buf);
            buf->sputc(']');
        };
    };
    //simplify
    template<typename T>
    struct parser<T, char, std::enable_if_t<same<T,My_type>, void>> {
        template<typename traits,typename Buf>
        static void parse(T&& value, traits* trait, Buf* buf) {
            trait->format("[%,%,%]",value.i,value.f,value.b);
        };
    };
}
```
