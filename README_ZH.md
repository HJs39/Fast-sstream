# `fsstream`函式庫

`fsstream`是一個"header-only"的函式庫，它提供類標準的字串流，你可以特化`parser`和`scanner`類以支援你的類別

你可以透過簡單的將命名空間從`std`更改為`fast_sstream`來替換當前使用的標準字串流

使用此函式庫的最低版本為`C++17`

## 包含
```cpp
#include "fsstream.hpp"
using namespace fast_sstream; // 這會汙染你的命名空間
using fast_sstream::stringstream; // 請改用這個
```

## 特色
* 比標準字串流更快
* 可以直接輸出至標準輸出流(`stdout`或`std::basic_ostream`)
* 類標準的API

## 使用

```cpp
fast_sstream::stringstream ss;
//插入
ss<<"Hello world\n"<<39<<true<<0.01<<'\n';
//輸出
std::cout<<ss.view()<<ss.str();
ss>>std::cout>>ss>>stdout;
```
結果:
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

## 限制
* 非線程安全
* 不支援`std::locale`
* 指原生支持`char`型別
