# `fsstream`庫

[english version](https://github.com/HJs39/Fast-sstream/blob/36295a47a9d40982038dfbbda34a83a84036845f/README.md)

`fsstream`是一個"header-only"的字串流庫，提供類標準的API流以進行字串與值得相互轉換，你可以特化`parser`與`scanner`類來支援你的自訂型別。

你可以將`std`命名空間更改為`fast_sstream`來將標準流替換為`fsstream`庫

## 限制

* 需要`C++17`以上
* 非線程安全
* 不支援`std::locale`
* 僅支援`char`型別

## 特色

* 比標準`sstream`庫更快
* 可直接向標準輸出流輸出資料(比如`stdout`與`std::basic_ostream`)
* 類標準的API

## 使用

### 包含

```cpp
#include "fsstream.hpp"

using namespace fast_sstream;
//^這會把太多的空結構與別名導入你的空間

//建議使用:
using fast_sstream::stringstream;
using fast_sstream::istringstream;
using fast_sstream::ostringstream;
//或
namespace fss = fast_sstream;
```

### Basic using

你可以像標準字串流一般創建並使用它(但在初始建構中傳入字串將引用而非複製)

```cpp
fast_sstream::stringstream ss;
ss << "Hello world " << 39 << ' ' << true << ' ' << 0.01 << '\n';
std::cout << ss.view();//or ss.str()
```

輸出:

```text
Hello world 39 true 0.010000
```

你可以使用`fast_sstream`中提供的操作子以修改流的狀態

```cpp
fast_sstream::stringstream ss;
ss << "Before setting:" << "\ninteger:" << 39 << "\nboolean:" << true << "\nfloating point:" << 1.732;
ss << fast_sstream::noboolalpha << fast_sstream::setprecision(5) << fast_sstream::hex;
ss<<"\nAfter setting:"<< "\ninteger:" << 39 << "\nboolean:" << true << "\nfloating point:" << 1.732;
std::cout << ss.view();
```

輸出:

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

如果需要輸出資料，可以使用`operator>>`對標準流的特化

```cpp
fast_sstream::stringstream ss;
ss << "output test\n";
ss >> stdout >> std::cout;
```

輸出:

```text
output test
output test
```

使用`fast_sstream::nostdcpy`來消耗緩衝區資料而非複製它

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

你可以向`fast_sstream`命名空間添加`parser`與`scanner`的特化來支援你的自訂型別

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
            //            ^第一項為接收參數
            //             第二項為呼叫parse函式的I/O特徵，有需要可以使用內部的任何成員，但不推薦
            //             第三項為I/O特徵持有的底層緩衝，你同樣可以使用內部的任何成員，但不推薦
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
            //           ^與parser::parse一致，但第一項為結果
            buf->gbump(1);
            fast_sstream::scanner<int, char>::scan(value.i, trait, buf);
            buf->gbump(1);
            fast_sstream::scanner<float, char>::scan(value.f, trait, buf);
            buf->gbump(1);
            fast_sstream::scanner<bool, char>::scan(value.b, trait, buf);
            buf->gbump(1);
            //上面的只是一個簡單的範例，你可以用更多的檢查和拋擲異常來保證結果準確性
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

輸出:

```text
[39,0.555,false]
scan result:[39,0.555,0]
```

Tips: 目前`parser::parse`需要使用`std::forward`，這個問題會在未來修復
