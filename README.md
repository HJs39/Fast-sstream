# `fsstream` library

`fsstream` is a header-only string stream library, it provides standard-like string stream to translate various values to characters, and you can specialize the `parser` and `scanner` classes to support your custom types.

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
//^Don't do this, it will pollute your namespace with too many empty structures and alias

//Recommend:
using fast_sstream::stringstream;
using fast_sstream::istringstream;
using fast_sstream::ostringstream;
//or
namespace fss = fast_sstream;

```

### Basic using

You can construct a empty stream and use it like standard provides.

```cpp

fast_sstream::stringstream ss;
ss << "Hello world " << 39 << ' ' << true << ' ' << 0.01 << '\n';
std::cout << ss.view();//or ss.str()

```

output:

```text

Hello world 39 true 0.010000

```

You can use manipulators like standard provides to switch default stream setting.

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

If want to output buffer data, you can use `operator>>` specialized for standard stream.

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

using `fast_sstream::nostdcpy` manipulator to comsume the data.

```cpp

fast_sstream::stringstream ss;
ss << "output test\n" << fast_sstream::nostdcpy;
ss >> stdout;
ss << 3939;
ss >> std::cout;

```text

output:

```text

output test
3939

```

And you can add your `parser` and `scanner` specialization to `fast_sstream` namespace to support custom type.

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
            //            ^first: your type.
            //             second:trait that calls the parse, you can use all members because parser is it's friend.
            //             third:the buffer that traits is holding, you can use all members, but is not recommended.
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
            //           ^equal to comment of parser, buf the first argument is to get the result you 
            //            scan.
            buf->gbump(1);
            fast_sstream::scanner<int, char>::scan(value.i, trait, buf);
            buf->gbump(1);
            fast_sstream::scanner<float, char>::scan(value.f, trait, buf);
            buf->gbump(1);
            fast_sstream::scanner<bool, char>::scan(value.b, trait, buf);
            buf->gbump(1);
            //this is just an example, you can add more checking to ensure the result is correct.
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
scan result:[39,0.555,0]

```

Tips: the `parser::parse` need to call by `std::forward` now, but I will fixed this in future.
