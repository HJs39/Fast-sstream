// -*- C++ -*-
//===------------------------- streambuf ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//^ use libc++ std::basic_streambuf function members
//source code at:https://github.com/google/libcxx
//see libc++ licenses at LICENSE_libc++.TXT
// 
//my licenses at LICENSE.TXT
#pragma once
#ifndef _FAST_SSTREAM_
#define _FAST_SSTREAM_
#include <ios> //for std::ios_base
#include <memory> //for std::allocator
#include <string_view> //for std::strin_view
#include <string> //for std::string std::char_traits
#include <cstring> //for std::memcpy
#include <charconv> //for std::from_chars std::to_chars
#include <type_traits> //for std::is_same std::is_integral std::is_floating_point 
                       //std::enable_if_t std::is_base_of_v std::remove_cv_t std::remove_reference_t
#include <exception> //for std::exception
#include <sstream> //for std::istringstream
#include <ostream> //for std::basic_ostream
#include <array> //for std::array

#ifdef _MSC_VER
#define __EMP_BASE __declspec(empty_bases)
#else
#define __EMP_BASE
#endif
namespace fast_sstream {
    namespace default_func_detail {
        struct xsputn_t {
            template<typename source>
            inline static std::streamsize xsputn(source* p, const typename source::char_type* s, std::streamsize size) {
                std::streamsize _s = 0;
                for (; _s < size; ++s, ++_s) {
                    if (p->pptr() < p->epptr()) {
                        *p->pptr() = *s;
                        p->pbump(1);
                    }
                    else if (p->overflow(source::traits_type::to_int_type(*s)) == source::traits_type::eof()) {
                        break;
                    }
                }
                return _s;
            };
        };

        struct xsgetn_t {
            template<typename source>
            inline static std::streamsize xsgetn(source* p, typename source::char_type* s, std::streamsize size) {
                std::streamsize _s = 0;
                typename source::int_type _c;
                for (; _s < size; ++s, ++_s) {
                    if (p->gptr() < p->epptr()) *s = p->sbumpc();
                    else if ((_c = p->uflow()) != source::traits_type::eof()) *s = source::traits_type::to_char_type(_c);
                    else break;
                }
                return _s;
            }
        };

        struct xoverflow_t {
            template<typename source>
            inline static typename source::int_type xoverflow(source* p, typename source::int_type ch = source::traits_type::eof())noexcept(true) {
                return source::traits_type::eof();
            };
        };

        struct xunderflow_t {
            template<typename source>
            inline static typename source::int_type xunderflow(source* p)noexcept(true) {
                return source::traits_type::eof();
            };
        };

        struct xuflow_t {
            template<typename source>
            inline static typename source::int_type xuflow(source* p)noexcept(noexcept(p->underflow())) {
                typename source::int_type result = p->underflow();
                if (!source::traits_type::eq_int_type(result, source::traits_type::eof()))p->gbump(1);
                return result;
            }
        };

        struct xpbackfail_t {
            template<typename source>
            inline static typename source::int_type xpbackfail(source* p, typename source::int_type c = source::traits_type::eof())noexcept(true) {
                return source::traits_type::eof();
            };
        };

        struct sync_t {
            template<typename source>
            inline static int sync(source* p)noexcept(true) {
                return 0;
            };
        };

        struct setbuf_t {
            template<typename source>
            inline static source* setbuf(source* p, typename source::char_type* s, std::streamsize size)noexcept(true) {
                return p;
            };
        };

        struct seekoff_t {
            template<typename source>
            inline static typename source::pos_type seekoff(source* p, typename source::off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which)noexcept(true) {
                return typename source::pos_type{ typename source::off_type{-1} };
            };
        };

        struct seekpos_t {
            template<typename source>
            inline static typename source::pos_type seekpos(source* p, typename source::pos_type pos, std::ios_base::openmode which)noexcept(true) {
                return typename source::pos_type{ typename source::off_type{-1} };
            };
        };

        struct showmanyc_t {
            template<typename source>
            inline static int xshowmanyc(source* p)noexcept(true) {
                return 0;
            };
        };

    };//default_func_detail
    using xsputn_f = default_func_detail::xsputn_t;

    using xsgetn_f = default_func_detail::xsgetn_t;

    using xoverflow_f = default_func_detail::xoverflow_t;

    using xunderflow_f = default_func_detail::xunderflow_t;

    using xuflow_f = default_func_detail::xuflow_t;

    using xpbackfail_f = default_func_detail::xpbackfail_t;

    using setbuf_f = default_func_detail::setbuf_t;

    using sync_f = default_func_detail::sync_t;

    using seekoff_f = default_func_detail::seekoff_t;

    using seekpos_f = default_func_detail::seekpos_t;

    using showmanyc_f = default_func_detail::showmanyc_t;


    namespace placeholder {

        template<typename check, typename... Types>
        inline constexpr bool has_type = (std::is_same_v<check, Types> || ...);

        struct xsputn_placeholder {
            template<typename CharT>
            [[maybe_unused]] inline std::streamsize xsputn([[maybe_unused]] const CharT* s, [[maybe_unused]] std::streamsize size)noexcept(true) { return 0; };
        };
        template<typename...Ts>
        inline constexpr bool derived_xsputn = has_type<xsputn_placeholder, Ts...>;

        struct xsgetn_placeholder {
            template<typename CharT>
            [[maybe_unused]] inline std::streamsize xsgetn([[maybe_unused]] CharT* s, [[maybe_unused]] std::streamsize size)noexcept(true) { return 0; };
        };
        template<typename...Ts>
        inline constexpr bool derived_xsgetn = has_type<xsgetn_placeholder, Ts...>;

        struct xoverflow_placeholder {
            template<typename CharT>
            [[maybe_unused]] inline int xoverflow(CharT c)noexcept(true) { return 0; };
        };
        template<typename...Ts>
        inline constexpr bool derived_xoverflow = has_type<xoverflow_placeholder, Ts...>;

        struct xunderflow_placeholder {
            [[maybe_unused]] inline int xunderflow()noexcept(true) { return 0; };
        };
        template<typename...Ts>
        inline constexpr bool derived_xunderflow = has_type<xunderflow_placeholder, Ts...>;

        struct xuflow_placeholder {
            [[maybe_unused]] inline int xuflow()noexcept(true) { return 0; };
        };
        template<typename...Ts>
        inline constexpr bool derived_xuflow = has_type<xuflow_placeholder, Ts...>;

        struct xpbackfail_placeholder {
            template<typename IntT>
            [[maybe_unused]] IntT xpbackfail([[maybe_unused]] IntT c)noexcept(true) { return 0; };
        };
        template<typename...Ts>
        inline constexpr bool derived_xpbackfail = has_type<xpbackfail_placeholder, Ts...>;

        struct sync_placeholder {
            [[maybe_unused]] inline int sync()noexcept(true) { return 0; };
        };
        template<typename...Ts>
        inline constexpr bool derived_sync = has_type<sync_placeholder, Ts...>;

        struct setbuf_placeholder {
            template<typename CharT>
            [[maybe_unused]] inline std::nullptr_t setbuf([[maybe_unused]] CharT* s, [[maybe_unused]] std::streamsize size)noexcept(true) { return nullptr; };
        };
        template<typename...Ts>
        inline constexpr bool derived_setbuf = has_type<setbuf_placeholder, Ts...>;

        struct seekoff_placeholder {
            template<typename off_type>
            [[maybe_unused]] inline int seekoff([[maybe_unused]] off_type off, [[maybe_unused]] std::ios_base::seekdir dir, [[maybe_unused]] std::ios_base::openmode which)noexcept(true) { return 0; };
        };
        template<typename...Ts>
        inline constexpr bool derived_seekoff = has_type<seekoff_placeholder, Ts...>;

        struct seekpos_placeholder {
            template<typename pos_type>
            [[maybe_unused]] inline pos_type seekpos([[maybe_unused]] pos_type pos, [[maybe_unused]] std::ios_base::openmode which)noexcept(true) { return 0; };
        };
        template<typename...Ts>
        inline constexpr bool derived_seekpos = has_type<seekpos_placeholder, Ts...>;

        struct xshowmanyc_placeholder {
            [[maybe_unused]] inline int xshowmanyc()noexcept(true) { return 0; };
        };
        template<typename...Ts>
        inline constexpr bool derived_xshowmanyc = has_type<xshowmanyc_placeholder, Ts...>;
    };//placeholder
    using xsputn_t = placeholder::xsputn_placeholder;
    template<typename...Ts>
    inline constexpr bool _xsputn = placeholder::derived_xsputn<Ts...>;

    using xsgetn_t = placeholder::xsgetn_placeholder;
    template<typename...Ts>
    inline constexpr bool _xsgetn = placeholder::derived_xsgetn<Ts...>;

    using xoverflow_t = placeholder::xoverflow_placeholder;
    template<typename...Ts>
    inline constexpr bool _xoverflow = placeholder::derived_xoverflow<Ts...>;

    using xunderflow_t = placeholder::xunderflow_placeholder;
    template<typename...Ts>
    inline constexpr bool _xunderflow = placeholder::derived_xunderflow<Ts...>;

    using xuflow_t = placeholder::xuflow_placeholder;
    template<typename...Ts>
    inline constexpr bool _xuflow = placeholder::derived_xuflow<Ts...>;

    using xpbackfail_t = placeholder::xpbackfail_placeholder;
    template<typename...Ts>
    inline constexpr bool _xpbackfail = placeholder::derived_xpbackfail<Ts...>;

    using setbuf_t = placeholder::setbuf_placeholder;
    template<typename...Ts>
    inline constexpr bool _setbuf = placeholder::derived_setbuf<Ts...>;

    using sync_t = placeholder::sync_placeholder;
    template<typename...Ts>
    inline constexpr bool _sync = placeholder::derived_sync<Ts...>;

    using seekoff_t = placeholder::seekoff_placeholder;
    template<typename...Ts>
    inline constexpr bool _seekoff = placeholder::derived_seekoff<Ts...>;

    using seekpos_t = placeholder::seekpos_placeholder;
    template<typename...Ts>
    inline constexpr bool _seekpos = placeholder::derived_seekpos<Ts...>;

    using xshowmanyc_t = placeholder::xshowmanyc_placeholder;
    template<typename...Ts>
    inline constexpr bool _xshowmanyc = placeholder::derived_xshowmanyc<Ts...>;

    enum class fmtflag :uint8_t {
        scientific = 1,
        fixed = 2,
        hex = 4,
        general = scientific | fixed,
        skipws = 8,
        boolalpha = 16,
        stdcpy = 32,
    };

    constexpr fmtflag operator|(fmtflag lt, fmtflag rt) {
        return static_cast<fmtflag>(static_cast<uint8_t>(lt) | static_cast<uint8_t>(rt));
    };
    constexpr fmtflag operator& (fmtflag lt, fmtflag rt) {
        return static_cast<fmtflag>(static_cast<uint8_t>(lt) & static_cast<uint8_t>(rt));
    };
    constexpr fmtflag operator~(fmtflag lt) {
        return static_cast<fmtflag>(~static_cast<uint8_t>(lt));
    };
    constexpr fmtflag& operator|=(fmtflag& lt, fmtflag rt) {
        lt = static_cast<fmtflag>(static_cast<uint8_t>(lt) | static_cast<uint8_t>(rt));
        return lt;
    };
    constexpr fmtflag& operator&=(fmtflag& lt, fmtflag rt) {
        lt = static_cast<fmtflag>(static_cast<uint8_t>(lt) & static_cast<uint8_t>(rt));
        return lt;
    };
    constexpr fmtflag& operator^=(fmtflag& lt, fmtflag rt) {
        lt = static_cast<fmtflag>(static_cast<uint8_t>(lt) ^ static_cast<uint8_t>(rt));
        return lt;
    };
    constexpr fmtflag operator^(fmtflag lt, fmtflag rt) {
        return static_cast<fmtflag>(static_cast<uint8_t>(lt) ^ static_cast<uint8_t>(rt));
    };

    constexpr fmtflag floatfield = fmtflag::fixed | fmtflag::scientific | fmtflag::hex;

    template<typename value_type, typename result_char_type, typename = void>
    //                ^parameter type     ^result char type         ^parameter type requires
    struct parser {
        //can be specialize to support custom type
        //no default support char/string type(default support in trait)
        static_assert(false, "specialize your type parser for custom type");
    };

    template<typename result_type, typename from_char_type, typename = void>
    //                ^to value type        ^buffer uses char type ^result type requires
    struct scanner {
        //can be specialize to support custom type
        static_assert(false, "specialize your type scanner for custom type");
    };

    template<typename T>
    using remove_cvref_t = typename std::remove_cv_t<std::remove_reference_t<T>>;

    template<typename Type>
    using is_integral = std::is_integral<remove_cvref_t<Type>>;
    template<typename Type>
    using is_floating_point = std::is_floating_point<remove_cvref_t<Type>>;
    template<typename Type, typename _Type>
    using is_same = std::is_same<remove_cvref_t<Type>, remove_cvref_t<_Type>>;

    template<typename Type, typename _Type>
    inline constexpr bool same = is_same<Type, _Type>::value;

    template<typename Type>
    inline constexpr bool integer = (is_integral<Type>::value && !(same<Type, char> ||
#ifdef __cpp_char8_t //char8_t
        same<Type, char8_t> ||
#endif
        same<Type, char16_t> ||
        same<Type, char32_t> ||
        same<Type, wchar_t> ||
        same<Type, bool>));

    template<typename Type>
    inline constexpr bool floating_point = is_floating_point<Type>::value;

    template<typename Type>
    struct parse_over_allocation_size {// default over-allocation determine
        inline constexpr static int size = 1;
    };

    template<>
    struct parse_over_allocation_size<unsigned short> {
        inline constexpr static int size = 5;
    };
    template<>
    struct parse_over_allocation_size<short> {
        inline constexpr static int size = 5;
    };
    template<>
    struct parse_over_allocation_size<unsigned int> {
        inline constexpr static int size = 5;
    };
    template<>
    struct parse_over_allocation_size<int> {
        inline constexpr static int size = 5;
    };
    template<>
    struct parse_over_allocation_size<unsigned long> {
        inline constexpr static int size = 5;
    };
    template<>
    struct parse_over_allocation_size<long> {
        inline constexpr static int size = 5;
    };
    template<>
    struct parse_over_allocation_size<unsigned long long> {
        inline constexpr static int size = 10;
    };
    template<>
    struct parse_over_allocation_size<long long> {
        inline constexpr static int size = 10;
    };

    std::chars_format get_format(fmtflag flag) {
        if ((flag & fmtflag::general) == fmtflag::general)return std::chars_format::general;
        if ((flag & fmtflag::scientific) == fmtflag::scientific)return std::chars_format::scientific;
        if ((flag & fmtflag::fixed) == fmtflag::fixed)return std::chars_format::fixed;
        if ((flag & fmtflag::hex) == fmtflag::hex)return std::chars_format::hex;
    }

    template<typename Integer>
    struct parser<Integer, char, std::enable_if_t<integer<Integer>, void>> {
        using traits_type = std::char_traits<char>;
        using ios = std::ios_base;
        template<typename input_output_trait, typename buffer>
        static void parse(Integer value, input_output_trait* trait, buffer* buf) {
            if (buf->pptr() == buf->epptr() && buf->overflow() == traits_type::eof()) {
                trait->setstate(ios::badbit, "parser: value too large");
                return;
            }
            else if (buf->epptr() - buf->pptr() < parse_over_allocation_size<Integer>::size) {
                buf->overflow();
            }
            auto result = std::to_chars(buf->pptr(), buf->epptr(), value, trait->base());
            if (result.ec != std::errc{}) {
                do {
                    if (buf->overflow() == traits_type::eof()) {
                        trait->setstate(ios::badbit, "parser: value too large");
                        return;
                    }
                    result = std::to_chars(buf->pptr(), buf->epptr(), value, trait->base());
                } while (result.ec != std::errc{});
            }
            buf->pbump(result.ptr - buf->pptr());
            trait->clear();
        };
    };

    template<typename Integer>
    struct scanner<Integer, char, std::enable_if_t<integer<Integer>, void>> {
        using traits_type = std::char_traits<char>;
        using ios = std::ios_base;
        template<typename input_output_trait, typename buffer>
        static void scan(Integer& value, input_output_trait* trait, buffer* buf) {
            if (trait->eof() || buf->gptr() == buf->egptr() && buf->underflow() == traits_type::eof()) {
                trait->setstate(ios::failbit, "scanner: no such data");
                return;
            }
            if (buf->gptr() != buf->egptr() && std::isalnum(*(buf->egptr() - 1)))buf->underflow();
            auto [ptr, errc] = std::from_chars(buf->gptr(), buf->egptr(), value, trait->base());
            if (errc == std::errc{}) {
                buf->setg(buf->eback(), const_cast<char*>(ptr), buf->egptr());
                if (ptr == buf->epptr())trait->setstate(ios::eofbit, "scanner: eof");
            }
            else {
                trait->setstate(ios::failbit, "scanner: failed to cast \"Integer\"");
            }
        }
    };

    template<typename floating_point_t>
    struct parser<floating_point_t, char, std::enable_if_t<floating_point<floating_point_t>, void>> {
        using traits_type = std::char_traits<char>;
        using ios = std::ios_base;
        template<typename input_output_trait, typename buffer>
        static void parse(floating_point_t value, input_output_trait* trait, buffer* buf) {
            if (buf->pptr() == buf->epptr() && buf->overflow() == traits_type::eof()) {
                trait->setstate(ios::badbit, "parser: value too large");
                return;
            }
            auto result = std::to_chars(buf->pptr(), buf->epptr(), value, get_format(trait->flag()), trait->precision());
            if (result.ec != std::errc{}) {
                do {
                    if (buf->overflow() == traits_type::eof()) {
                        trait->setstate(ios::badbit, "parser: value too large");
                        return;
                    }
                    result = std::to_chars(buf->pptr(), buf->epptr(), value, get_format(trait->flag()), trait->precision());
                } while (result.ec != std::errc{});
            }
            buf->pbump(result.ptr - buf->pptr());
            trait->clear();
        };
    };

    template<typename floating_point_t>
    struct scanner<floating_point_t, char, std::enable_if_t<floating_point<floating_point_t>, void>> {
        using traits_type = std::char_traits<char>;
        using ios = std::ios_base;
        template<typename input_output_trait, typename buffer>
        static void scan(floating_point_t& value, input_output_trait* trait, buffer* buf) {
            if (trait->eof() || buf->gptr() == buf->egptr() && buf->underflow() == traits_type::eof()) {
                trait->setstate(ios::failbit, "scanner: no such data");
                return;
            }
            if (buf->gptr() != buf->egptr() && (*(buf->egptr() - 1) == '.' || std::isalnum(*(buf->egptr() - 1))))buf->underflow();
            auto [ptr, errc] = std::from_chars(buf->gptr(), buf->egptr(), value, get_format(trait->flag()));
            if (errc == std::errc{}) {
                buf->setg(buf->eback(), const_cast<char*>(ptr), buf->egptr());
                if (ptr == buf->epptr())trait->setstate(ios::eofbit, "scanner: eof");
            }
            else {
                trait->setstate(ios::failbit, "scanner: failed to cast \"Floatint Point\"");
            }
        }
    };

    template<typename Boolean>
    struct parser<Boolean, char, std::enable_if_t<same<Boolean, bool>, void>> {
        using traits_type = std::char_traits<char>;
        using ios = std::ios_base;
        template<typename input_output_trait, typename buffer>
        static void parse(Boolean value, input_output_trait* trait, buffer* buf) {
            if ((trait->flag() & fmtflag::boolalpha) != fmtflag{}) {
                if (value) {
                    if (buf->epptr() - buf->pptr() < 4 && buf->overflow() == traits_type::eof()) {
                        trait->setstate(ios::badbit, "parser: value too large");
                        return;
                    }
                    buf->sputn("true", 4);
                }
                else {
                    if (buf->epptr() - buf->pptr() < 5 && buf->overflow() == traits_type::eof()) {
                        trait->setstate(ios::badbit, "parser: value too large");
                        return;
                    }
                    buf->sputn("false", 5);
                }
            }
            else {
                if (buf->epptr() - buf->pptr() < 1 && buf->overflow() == traits_type::eof()) {
                    trait->setstate(ios::badbit, "parser: value too large");
                    return;
                }
                if (value) {
                    buf->sputc('1');
                }
                else {
                    buf->sputc('0');
                }
            }
            trait->clear();
        };
    };

    template<typename Boolean>
    struct scanner<Boolean, char, std::enable_if_t<same<Boolean, bool>, void>> {
        using traits_type = std::char_traits<char>;
        using ios = std::ios_base;
        template<typename input_output_trait, typename buffer>
        static void scan(Boolean& value, input_output_trait* trait, buffer* buf) {
            if (buf->gptr() == buf->egptr() || trait->eof() && buf->underflow() == traits_type::eof()) {
                trait->setstate(ios::eofbit, "scanner: eof");
                return;
            }
            if ((trait->flag() & fmtflag::boolalpha) != fmtflag{}) {
                if (buf->egptr() - buf->gptr() < 4 && buf->underflow() == traits_type::eof()) {
                    trait->setstate(ios::failbit, "scanner: failed to cast \"Boolean\"");
                    return;
                }
                if (traits_type::compare(buf->gptr(), "true", 4) == 0) {
                    value = true;
                    buf->gbump(4);
                    return;
                }
                if (buf->egptr() - buf->gptr() < 5 && buf->underflow() == traits_type::eof()) {
                    trait->setstate(ios::failbit, "scanner: failed to cast \"Boolean\"");
                    return;
                }
                if (traits_type::compare(buf->gptr(), "false", 5) == 0) {
                    value = false;
                    buf->gbump(5);
                    return;
                }
            }
            else {
                if (buf->egptr() - buf->gptr() < 1 && buf->underflow() == traits_type::eof()) {
                    trait->setstate(ios::failbit, "scanner: failed to cast \"Boolean\"");
                    return;
                }
                if (traits_type::compare(buf->gptr(), "1", 1) == 0) {
                    value = true;
                    buf->gbump(1);
                    return;
                }
                if (traits_type::compare(buf->gptr(), "0", 1) == 0) {
                    value = false;
                    buf->gbump(1);
                    return;
                }
            }
            trait->setstate(ios::failbit, "scanner: failed to cast \"Boolean\"");
        }
    };

    template<typename CharType>
    struct scanner<CharType, CharType, std::enable_if_t<true, void>> {
        using traits_type = std::char_traits<CharType>;
        using ios = std::ios_base;
        template<typename input_output_trait, typename buffer>
        static void scan(CharType& ch, input_output_trait* trait, buffer* buf) {
            if (trait->eof() || buf->gptr() == buf->egptr() && buf->underflow() == traits_type::eof()) {
                trait->setstate(ios::failbit, "scanner: no such data");
                return;
            }
            ch = buf->sgetc();
            if (ch == traits_type::eof()) trait->setstate(ios::failbit | ios::eofbit, "scanner: failed to read data");
            else  buf->gbump(1);
        };

        template<typename input_output_trait, typename buffer>
        static void scan(CharType* ch, input_output_trait* trait, buffer* buf) {
            if (trait->eof() || buf->gptr() == buf->egptr() && buf->underflow() == traits_type::eof()) {
                trait->setstate(ios::failbit, "scanner: no such data");
                return;
            }
            CharType* _p = buf->gptr();
            CharType* _g = ch;
            std::streamsize count{ 0 };
            for (auto length{ traits_type::length(ch) }; count < length - 1;) {
                if (_p == buf->egptr() && buf->underflow() == traits_type::eof())break;
                if (std::isspace(*_p))break;
                *_g++ = *_p++;
                ++count;
            }
            buf->gbump(count);
            *_g = '\0';
            if (_g == ch) trait->setstate(ios::failbit, "scanner: failed to read string");
            if (buf->gptr() == buf->egptr())trait->setstate(ios::eofbit, "scanner: eof");
            return;
        };

        template<typename Traits, typename SAlloc, typename input_output_trait, typename buffer>
        static void scan(std::basic_string<CharType, Traits, SAlloc>& value, input_output_trait* trait, buffer* buf) {
            if (trait->eof() || buf->gptr() == buf->egptr() && buf->underflow() == traits_type::eof()) {
                trait->setstate(ios::failbit, "scanner: no such data");
                return;
            }
            value.clear();
            CharType* _p = buf->gptr();
            std::streamsize count{ 0 };
            while (true) {
                if (_p == buf->egptr() && buf->underflow() == traits_type::eof())break;
                if (std::isspace(*_p))break;
                value.push_back(*_p++);
                ++count;
            }
            buf->gbump(count);
            if (value.empty()) trait->setstate(ios::failbit, "scanner: failed to read string");
            if (buf->gptr() == buf->egptr())trait->setstate(ios::eofbit, "scanner: eof");
            return;
        }
    };

    template<typename stream>
    //output to C++ stream support
    struct scanner<stream, char, std::enable_if_t<std::is_base_of_v<std::basic_ostream<char>, stream> ||
        std::is_base_of_v<std::basic_iostream<char>, stream>, void>> {
        template<typename input_output_trait, typename Buf>
        static void scan(stream& dest, input_output_trait* trait, Buf* buf) {
            buf->underflow();
            if ((trait->flag() & fmtflag::stdcpy) != fmtflag{}) {
                dest.write(buf->eback(), buf->egptr() - buf->eback());
            }
            else {
                dest.write(buf->gptr(), buf->egptr() - buf->gptr());
                buf->gbump(buf->egptr() - buf->gptr());
            }
        };
    };

    struct check_format_result {
        std::size_t format_arg_num;
        std::uint64_t str_size;
    };

    template<typename CharT>
    check_format_result check_format(const CharT* str) {
        check_format_result result{ 0,0 };
        for (;; ++str) {
            if (*str == '\0') {
                return result;
            }
            else if (*str == '\\') {
                ++str;
                ++result.str_size;
            }
            else if (*str == '%' || *str == '^') {
                ++result.format_arg_num;
            }
            ++result.str_size;
        }
    };

    class ios_traits {
    private:
        using ios = std::ios_base;
        ios::iostate _state;
        ios::iostate _mask;
        fmtflag _flag;
        std::streamsize _precision;
        int _base;
    protected:
        ios_traits()noexcept :_state{ ios::goodbit }, _flag{ fmtflag::skipws | fmtflag::boolalpha | fmtflag::general | fmtflag::stdcpy }, _mask{}, _precision{ 6 }, _base{ 10 } {};
        void swap(ios_traits& other)noexcept(true) {
            fmtflag _ftmp = _flag;
            _flag = other._flag;
            other._flag = _ftmp;
            std::swap(_base, other._base);
            std::swap(_state, other._state);
            std::swap(_precision, other._precision);
            std::swap(_mask, other._mask);
        };
    public:
        //constructor
        ios_traits(const ios_traits&) = delete;
        ios_traits(ios_traits&& other)noexcept :_flag(std::move(other._flag)), _mask(std::move(other._mask)), _state(std::move(other._state)), _precision(std::move(other._precision)), _base(std::move(other._base)) {};

        //base
        inline int base()const noexcept { return _base; };
        void base(int n_base)noexcept {
            if (n_base < 2 || n_base>36)return;
            _base = n_base;
        };

        //precision
        inline std::streamsize precision()const noexcept { return _precision; };
        std::streamsize precision(std::streamsize new_precision) {
            std::streamsize _pre = _precision;
            _precision = new_precision;
            return _pre;
        };

        //ios state
        inline ios::iostate rdstate()const noexcept { return _state; };
        inline bool good()const noexcept { return _state & ios::goodbit; };
        inline bool eof()const noexcept { return _state & ios::eofbit; };
        inline bool fail()const noexcept { return _state & ios::failbit; };
        inline bool bad()const noexcept { return _state & ios::badbit; };
        inline void setstate(ios::iostate state, const char* mes) {
            _state |= state;
            if (_state & _mask)throw std::exception(mes);
        };
        inline void clear(ios::iostate state = ios::goodbit)noexcept { _state = state; };

        inline fmtflag flag()const noexcept { return _flag; };
        inline fmtflag setf(fmtflag flags)noexcept {
            fmtflag f = _flag;
            _flag |= flags;
            return f;
        };
        inline fmtflag setf(fmtflag flags, fmtflag mask)noexcept {
            fmtflag f = _flag;
            _flag = (_flag & ~mask) | (flags & mask);
            return f;
        };
        inline void unsetf(fmtflag flags)noexcept { _flag &= ~flags; };

        //exception
        inline ios::iostate exception()const noexcept(true) { return _mask; };
        inline void exception(ios::iostate except)noexcept(true) { _mask = except; };

        //operator
        inline bool operator!()const noexcept { return fail(); };
        inline operator bool()const noexcept { return !fail(); };
    };

    //flag manipulators
    ios_traits& boolalpha(ios_traits& stream)noexcept {
        stream.setf(fmtflag::boolalpha);
        return stream;
    };
    ios_traits& noboolalpha(ios_traits& stream)noexcept {
        stream.unsetf(fmtflag::boolalpha);
        return stream;
    };
    ios_traits& fixed(ios_traits& stream)noexcept {
        stream.setf(fmtflag::fixed, floatfield);
        return stream;
    };
    ios_traits& scientific(ios_traits& stream)noexcept {
        stream.setf(fmtflag::scientific, floatfield);
        return stream;
    };
    ios_traits& hex(ios_traits& stream)noexcept {
        stream.setf(fmtflag::hex, floatfield);
        return stream;
    };
    ios_traits& defaultfloat(ios_traits& stream)noexcept {
        stream.setf(fmtflag::general, floatfield);
        return stream;
    };
    ios_traits& skipws(ios_traits& stream)noexcept {
        stream.setf(fmtflag::skipws);
        return stream;
    };
    ios_traits& noskipws(ios_traits& stream)noexcept {
        stream.unsetf(fmtflag::skipws);
        return stream;
    };
    ios_traits& stdcpy(ios_traits& stream)noexcept {
        stream.setf(fmtflag::stdcpy);
        return stream;
    };
    ios_traits& nostdcpy(ios_traits& stream)noexcept {
        stream.unsetf(fmtflag::stdcpy);
        return stream;
    };

	//I/O manipulators support
    template<typename callable>
    struct parser<callable, char, std::enable_if_t<std::is_invocable_v<callable, ios_traits&>, void>> {
        template<typename input_output_trait,typename Buf>
        static void parse(callable&& func, input_output_trait* trait, Buf* buf) {
            func(*trait);
        };
    };
    template<typename callable>
    struct scanner<callable, char, std::enable_if_t<std::is_invocable_v<callable, ios_traits&>, void>> {
        template<typename input_output_trait, typename Buf>
        static void scan(callable&& func, input_output_trait* trait, Buf* buf) {
            func(*trait);
        };
    };
    //value manipulators
    inline auto setprecision(int pre) {
        return [pre](ios_traits& trait)->ios_traits& {
            trait.precision(pre);
            return trait;
            };
    };
    inline auto setbase(int num) {
        return [num](ios_traits& trait)->ios_traits& {
            trait.base(num);
            return trait;
            };
    };

    template<typename BufT, typename CharT, typename Traits = std::char_traits<CharT>>
    class output_traits :public ios_traits {
    private:
        using ios = std::ios_base;
        using char_type = CharT;
        using traits_type = Traits;
        using buf_type = BufT;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;
        buf_type* src;

        //consume any value matched by the format character '^'
        template<typename T>
        inline constexpr void consume(T&&)const noexcept {
            return;
        };

        bool write_fmt(const char_type*& fmt, const char_type* end_pos) {
            if (this->fail())return false;//fail to format, skip argument
            for (;; ++fmt) {
                if (fmt >= end_pos)return false;
                else if (*fmt == '\\') {
                    ++fmt;
                    if (src->sputc(*fmt) == traits_type::eof()) {
                        this->setstate(ios::failbit, "output_traits::write_fmt:no such memory");
                        return false;
                    }
                }
                else if (*fmt == '%') {
                    ++fmt;
                    return true;//can format argument
                }
                else if (*fmt == '^') {
                    ++fmt;
                    return false;//skip format argument
                }
                else {
                    if (src->sputc(*fmt) == traits_type::eof()) {
                        this->setstate(ios::failbit, "output_traits::write_fmt:no such memory");
                        return false;
                    }
                }
            }
        };

        template<typename, typename, typename>
        friend struct parser;
    protected:
        output_traits() = delete;
        output_traits(buf_type* associate) :src{ associate }, ios_traits{} {};
        output_traits(output_traits&& other) :src{ std::move(other.src) }, ios_traits{ std::move(other) } {};
        void swap(output_traits& other)noexcept(true) {
            ios_traits::swap(other);
            std::swap(src, other.src);
        };
    public:

        //write
        output_traits& put(char_type ch) {
            if (src->sputc(ch) == traits_type::eof()) {
                this->setstate(ios::badbit, "output_traits::put: no such memory");
            }
            else {
                this->clear();
            }
            return *this;
        };
        output_traits& write(const char_type* s, std::streamsize count) {
            if (src->sputn(s, count) != count) {
                this->setstate(ios::badbit, "output_traits::put: no such memory");
            }
            else {
                this->clear();
            }
            return *this;
        };

        //use '%' to format a argument to buffer
        //use '^' to ignore a argument from pack
        //use '\'to escape the formatting character
        template<typename... Args>
        io_traits& format(const CharT* fmt, Args&&...args) {
            auto [fmt_count, str_size] = check_format(fmt);
            if (fmt_count > sizeof...(args)) {
                this->setstate(ios::badbit, "output_traits::format:no such arguments");
                return *this;
            }
            const CharT* fmt_pos = fmt;
            const CharT* fmt_end = fmt + str_size;
            ((write_fmt(fmt_pos, fmt_end)), ..., (write_fmt(fmt_pos, fmt_end) ? static_cast<void>((*this << std::forward<Args>(args))) : consume(std::forward<Args>(args))));
            return *this;
        };

        //seek
        pos_type tellp() {
            if (this->fail())return pos_type{ -1 };
            else return src->pubseekoff(0, ios::cur, ios::out);
        };
        output_traits& seekp(pos_type pos) {
            if (!this->fail()) src->pubseekpos(pos, ios::out);
            else this->setstate(ios::failbit, "output_traits::seekp: fail to seek");
            return *this;
        };
        output_traits& seekp(off_type off, ios::seekdir dir) {
            if (this->fail() || src->pubseekoff(off, dir, ios::out) == pos_type{ off_type{-1} }) this->setstate(ios::failbit, "output_traits::seekp: fail to seek");
            return *this;
        }

        //operator

        //for the type that specialized the parser
        template<typename Type>
        output_traits& operator<<(Type&& value) {
            parser<Type, char_type>::parse(std::forward<Type>(value), this, src);
            return *this;
        }
        output_traits& operator<<(const char_type value) {
            if (src->sputc(value) == traits_type::eof()) {
                this->setstate(ios::badbit, "output_traits: no such memory");
            }
            else {
                this->clear();
            }
            return *this;
        };
        output_traits& operator<<(const char_type* value) {
            if (src->sputn(value, traits_type::length(value)) != traits_type::length(value)) {
                this->setstate(ios::badbit, "output_traits: value too large");
            }
            else {
                this->clear();
            }
            return *this;
        };
        template<std::size_t N>
        output_traits& operator<<(const char_type(&value)[N]) {
            if (src->sputn(value, N) != N) {
                this->setstate(ios::badbit, "output_traits: value too large");
            }
            else {
                this->clear();
            }
            return *this;
        };
        template<typename Alloc>
        output_traits& operator<<(const std::basic_string<CharT, Traits, Alloc>& value) {
            if (src->sputn(value.c_str(), value.size()) != value.size()) {
                this->setstate(ios::badbit, "output_traits: value too large");
            }
            else {
                this->clear();
            }
            return *this;
        };
        output_traits& operator<<(std::basic_string_view<CharT, Traits> value) {
            if (src->sputn(value.data(), value.size()) != value.size()) {
                this->setstate(ios::badbit, "output_traits: value too large");
            }
            else {
                this->clear();
            }
            return *this;
        };
        output_traits& operator<<(ios_traits& (*func)(ios_traits&)) {
            func(*this);
            return *this;
        };
        output_traits& operator<<(ios_traits& (&func)(ios_traits&)) {
            func(*this);
            return *this;
        };
    };

    template<typename BufT, typename CharT, typename Traits = std::char_traits<CharT>>
    class input_traits :public ios_traits {
    private:
        using char_type = CharT;
        using traits_type = Traits;
        using int_type = typename Traits::int_type;
        using buf_type = BufT;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;
        template<typename SAlloc>
        using String = std::basic_string<CharT, Traits, SAlloc>;
        using StringView = std::basic_string_view<CharT, Traits>;
        using ios = std::ios_base;
        buf_type* src;

        template<typename, typename, typename>
        friend struct scanner;

        void skipws() {
            while (std::isspace(*src->gptr())) {
                src->gbump(1);
                if (src->gptr() == src->egptr() && src->underflow() == traits_type::eof()) {
                    this->setstate(ios::eofbit, "input_traits::skipws: eof");
                    return;
                }
            }
        };
        std::streamsize _gcount;
    protected:
        input_traits() = delete;
        input_traits(buf_type* associate)noexcept :ios_traits{}, src{ associate }, _gcount{ 0 } {};
        input_traits(input_traits&& other) :_gcount(0), src(std::move(other.src)), ios_traits(std::move(other)) {};
        void swap(input_traits& other)noexcept(true) {
            ios_traits::swap(other);
            buf_type* _tmp = src;
            src = other.src;
            other.src = _tmp;
            std::swap(_gcount, other._gcount);
        };

    public:
        inline input_traits& putback(char_type ch)noexcept(noexcept(src->sputbackc(ch))) {
            _gcount = 0;
            src->sputbackc(ch);
            return *this;
        };

        input_traits& ignore(std::streamsize count = 1, int_type delim = traits_type::eof()) {
            std::streamsize _count{ 0 };
            for (; count >= _count; ++_count) {
                if (traits_type::eq_int_type(src->sgetc(), delim)) {
                    _gcount = _count;
                    if (delim == traits_type::eof()) this->setstate(ios::eofbit, "input_traits::ignore: eof");
                    return *this;
                }
                else if (traits_type::eq_int_type(src->sgetc(), traits_type::eof())) {
                    _gcount = _count;
                    this->setstate(ios::eofbit, "input_traits::ignore: eof");
                    return *this;
                }
            }
            _gcount = _count;
            return *this;
        };

        input_traits& read(char_type* s, std::streamsize count) {
            char_type* _p = src->gptr();
            std::streamsize _count{ 0 };
            if (src->egptr() - _p < count) {
                if (src->underflow() == traits_type::eof()) {
                    _count = src->egptr() - _p;
                }
                else {
                    _count = src->egptr() - _p > count ? count : src->egptr() - _p;
                }
            }
            else {
                _count = count;
            }
            traits_type::copy(s, _p, _count);
            _gcount = _count;
            if (_count < count)this->setstate(ios::failbit | ios::eofbit, "input_traits::read: no such data");
            return *this;
        };

        input_traits& getline(char_type* s, std::streamsize count, char_type delim) {
            if (count < 0) {
                this->setstate(ios::failbit);
                return *this;
            }
            char_type* _p = src->gptr();
            char_type* _s = s;
            std::streamsize _count{ 0 };
            for (; _count < count - 1; ++_count, ++_p, ++_s) {
                if (_p == src->egptr() && src->underflow() == traits_type::eof()) {
                    _gcount = _count;
                    src->gbump(_count);
                    s[_count] = '\0';
                    this->setstate(ios::failbit, "input_traits::getline: no such data");
                    return *this;
                }
                if (traits_type::eq(*_p, delim)) {
                    ++_count;
                    break;
                }
                else {
                    *_s = *_p;
                }
            }
            _gcount = _count;
            src->gbump(_count);
            s[_count] = '\0';
            return *this;
        };
        inline input_traits& getline(char_type* s, std::streamsize count) {
            return getline(s, count, '\n');
        };
        int_type get() {
            int_type _r = src->sgetc();
            if (!traits_type::eq_int_type(_r, traits_type::eof()))src->gbump(1);
            else this->setstate(ios::failbit, "input_traits::get: no such data");
            return _r;
        };
        input_traits& get(char_type& c) {
            if (traits_type::eq_int_type(src->sgetc(), traits_type::eof())) {
                c = traits_type::to_char_type(traits_type::eof());
            }
            else {
                c = *src->gptr();
                src->gbump(1);
            }
            return *this;
        };

        input_traits& unget() {
            _gcount = 0;
            if (traits_type::eq_int_type(src->sungetc(), traits_type::eof()))this->setstate(ios::failbit, "input_tarits::ungetc: no such data");
            return *this;
        };

        int_type peek() {
            if (this->good())return src->sgetc();
            return traits_type::eof();
        };

        inline std::streamsize gcount() const noexcept(true) {
            return _gcount;
        };

        int sync() {
            if (src->pubsync() != -1)return 0;
            this->setstate(ios::badbit, "input_traits::sync: bad sync called");
            return -1;
        };

        inline pos_type tellg()noexcept(noexcept(src->pubseekoff(0, ios::cur, ios::in))) {
            return src->pubseekoff(0, ios::cur, ios::in);
        };

        input_traits& seekg(pos_type pos) {
            this->unsetf(this->rdstate() & ~ios::eofbit);
            if (this->fail())return *this;
            try {
                src->pubseekpos(pos, ios::in);
            }
            catch (...) {
                this->setstate(ios::badbit, "input_tarits::seekg: bad position seek");
            }
            return *this;
        };
        input_traits& seekg(off_type off, ios::seekdir dir) {
            this->unsetf(this->rdstate() & ~ios::eofbit);
            if (this->fail())return *this;
            try {
                src->pubseekoff(off, dir, ios::in);
            }
            catch (...) {
                this->setstate(ios::badbit, "input_tarits::seekg: bad position seek");
            }
            return *this;
        };

        /*operator*/

        //for the type that specialized the scanner
        template<typename Type>
        input_traits& operator>>(Type& value) {
            if ((this->flag() & fmtflag::skipws) != fmtflag{})this->skipws();
            scanner<Type, char_type>::scan(value, this, src);
            return *this;
        };
        template<typename Trait, typename SAlloc>
        input_traits& operator>>(std::basic_string<char_type, Trait, SAlloc>& value) {
            if ((this->flag() & fmtflag::skipws) != fmtflag{})this->skipws();
            scanner<char_type, char_type>::scan(value, this, src);
            return *this;
        };
        input_traits& operator>>(std::istringstream& iss) {
            if ((this->flag() & fmtflag::skipws) != fmtflag{})this->skipws();
            if ((this->flag() & fmtflag::stdcpy) == fmtflag{}) {
                iss.str(src->view());
            }
            else {
                StringView view = src->view();
                view.remove_prefix(src->gptr() - src->eback());
                iss.str(view);
            }
            return *this;
        };
        //C-type I/O support
        input_traits& operator>>(std::FILE* dest) {
            if ((this->flag() & fmtflag::skipws) != fmtflag{})this->skipws();
            src->underflow();
            if ((this->flag() & fmtflag::stdcpy) != fmtflag{}) {
                std::fwrite(src->eback(), sizeof(char_type), src->egptr() - src->eback(), dest);
            }
            else {
                std::fwrite(src->gptr(), sizeof(char_type), src->egptr() - src->gptr(), dest);
                src->gbump(src->egptr() - src->gptr());
            }
            return *this;
        };
        input_traits& operator>>(ios_traits& (*func)(ios_traits&))noexcept {
            func(*this);
            return *this;
        };
        input_traits& operator>>(ios_traits& (&func)(ios_traits&))noexcept {
            func(*this);
            return *this;
        };
    };

    template<typename BufT, typename CharT, typename Traits = std::char_traits<CharT>>
    class io_traits :public ios_traits {
    private:
        using ios = std::ios_base;
        using char_type = CharT;
        using traits_type = Traits;
        using int_type = typename traits_type::int_type;
        using StringView = std::basic_string_view<CharT, Traits>;
        using buf_type = BufT;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;
        buf_type* src;
        std::streamsize _gcount;

        template<typename, typename, typename>
        friend struct parser;
        template<typename, typename, typename>
        friend struct scanner;

        void skipws() {
            while (std::isspace(*src->gptr())) {
                src->gbump(1);
                if (src->gptr() == src->egptr() && src->underflow() == traits_type::eof()) {
                    this->setstate(ios::eofbit, "io_traits::skipws: eof");
                    return;
                }
            }
        };

        //consume any value matched by the format character '^'
        template<typename T>
        inline constexpr void consume(T&&)const noexcept {
            return;
        };

        bool write_fmt(const char_type*& fmt, const char_type* end_pos) {
            if (this->fail())return false;//fail to format, skip argument
            for (;; ++fmt) {
                if (fmt >= end_pos)return false;
                else if (*fmt == '\\') {
                    ++fmt;
                    if (src->sputc(*fmt) == traits_type::eof()) {
                        this->setstate(ios::failbit, "io_traits::write_fmt:no such memory");
                        return false;
                    }
                }
                else if (*fmt == '%') {
                    ++fmt;
                    return true;//can format argument
                }
                else if (*fmt == '^') {
                    ++fmt;
                    return false;//skip format argument
                }
                else {
                    if (src->sputc(*fmt) == traits_type::eof()) {
                        this->setstate(ios::failbit, "io_traits::write_fmt:no such memory");
                        return false;
                    }
                }
            }
        };

    protected:
        io_traits() = delete;
        io_traits(buf_type* buf) :src{ buf }, _gcount{ 0 }, ios_traits{} {};
        io_traits(io_traits&& other) :src{ std::move(other.src) }, _gcount{ std::move(other._gcount) }, ios_traits{ std::move(other) } {};
        void swap(io_traits& other)noexcept(true) {
            std::swap(src, other.src);
            std::swap(_gcount, other._gcount);
            ios_traits::swap(other);
        };
    public:
        inline io_traits& putback(char_type ch)noexcept(src->sputbackc(ch)) {
            _gcount = 0;
            src->sputbackc(ch);
            return *this;
        };

        io_traits& ignore(std::streamsize count = 1, int_type delim = traits_type::eof()) {
            std::streamsize _count{ 0 };
            for (; count >= _count; ++_count) {
                if (traits_type::eq_int_type(src->sgetc(), delim)) {
                    _gcount = _count;
                    if (delim == traits_type::eof()) this->setstate(ios::eofbit, "io_traits::ignore: eof");
                    return *this;
                }
                else if (traits_type::eq_int_type(src->sgetc(), traits_type::eof())) {
                    _gcount = _count;
                    this->setstate(ios::eofbit, "io_traits::ignore: eof");
                    return *this;
                }
            }
            _gcount = _count;
            return *this;
        };

        io_traits& read(char_type* s, std::streamsize count) {
            char_type* _p = src->gptr();
            std::streamsize _count{ 0 };
            if (src->egptr() - _p < count) {
                if (src->underflow() == traits_type::eof()) {
                    _count = src->egptr() - _p;
                }
                else {
                    _count = src->egptr() - _p > count ? count : src->egptr() - _p;
                }
            }
            else {
                _count = count;
            }
            traits_type::copy(s, _p, _count);
            _gcount = _count;
            if (_count < count)this->setstate(ios::failbit | ios::eofbit, "io_traits::read: no such data");
            return *this;
        };

        io_traits& getline(char_type* s, std::streamsize count, char_type delim) {
            if (count < 0) {
                this->setstate(ios::failbit);
                return *this;
            }
            char_type* _p = src->gptr();
            char_type* _s = s;
            std::streamsize _count{ 0 };
            for (; _count < count - 1; ++_count, ++_p, ++_s) {
                if (_p == src->egptr() && src->underflow() == traits_type::eof()) {
                    _gcount = _count;
                    src->gbump(_count);
                    s[_count] = '\0';
                    this->setstate(ios::failbit, "io_traits::getline: no such data");
                    return *this;
                }
                if (traits_type::eq(*_p, delim)) {
                    ++_count;
                    break;
                }
                else {
                    *_s = *_p;
                }
            }
            _gcount = _count;
            src->gbump(_count);
            s[_count] = '\0';
            return *this;
        };
        inline io_traits& getline(char_type* s, std::streamsize count) {
            return getline(s, count, '\n');
        };
        int_type get() {
            int_type _r = src->sgetc();
            if (!traits_type::eq_int_type(_r, traits_type::eof()))src->gbump(1);
            else this->setstate(ios::failbit, "io_traits::get: no such data");
            return _r;
        };
        io_traits& get(char_type& c) {
            if (traits_type::eq_int_type(src->sgetc(), traits_type::eof())) {
                c = traits_type::to_char_typy(traits_type::eof());
            }
            else {
                c = *src->gptr();
                src->gbump(1);
            }
            return *this;
        };

        io_traits& unget() {
            _gcount = 0;
            if (traits_type::eq_int_type(src->sungetc(), traits_type::eof()))this->setstate(ios::failbit, "input_tarits::ungetc: no such data");
            return *this;
        };

        int_type peek() {
            _gcount = 0;
            if (this->good())return src->sgetc();
            return traits_type::eof();
        };

        inline std::streamsize gcount() const noexcept(true) {
            return _gcount;
        };

        int sync() {
            if (src->pubsync() != -1)return 0;
            this->setstate(ios::badbit, "io_traits::sync: bad sync called");
            return -1;
        };

        inline pos_type tellg()noexcept(noexcept(src->pubseekoff(0, ios::cur, ios::in))) {
            return src->pubseekoff(0, ios::cur, ios::in);
        };

        io_traits& seekg(pos_type pos) {
            this->unsetf(ios::eofbit);
            if (this->fail())return *this;
            try {
                src->pubseekpos(pos, ios::in);
            }
            catch (...) {
                this->setstate(ios::badbit, "io_traits::seekg: bad position seek");
            }
            return *this;
        };
        io_traits& seekg(off_type off, ios::seekdir dir) {
            this->unsetf(ios::eofbit);
            if (this->fail())return *this;
            try {
                src->pubseekoff(off, dir, ios::in);
            }
            catch (...) {
                this->setstate(ios::badbit, "io_traits::seekg: bad position seek");
            }
            return *this;
        };

        //write
        io_traits& put(char_type ch) {
            if (src->sputc(ch) == traits_type::eof()) {
                this->setstate(ios::badbit, "io_traits::put: no such memory");
            }
            else {
                this->clear();
            }
            return *this;
        };
        io_traits& write(const char_type* s, std::streamsize count) {
            if (src->sputn(s, count) != count) {
                this->setstate(ios::badbit, "io_traits::put: no such memory");
            }
            else {
                this->clear();
            }
            return *this;
        };

        //use '%' to format a argument to buffer
        //use '^' to ignore a argument from pack
        //use '\' to escape the formatting character
        template<typename... Args>
        io_traits& format(const CharT* fmt, Args&&...args) {
            auto [fmt_count, str_size] = check_format(fmt);
            if (fmt_count > sizeof...(args)) {
                this->setstate(ios::badbit, "io_traits::format:no such arguments");
                return *this;
            }
            const CharT* fmt_pos = fmt;
            const CharT* fmt_end = fmt + str_size;
            ((write_fmt(fmt_pos, fmt_end)), ..., (write_fmt(fmt_pos, fmt_end) ? static_cast<void>((*this << std::forward<Args>(args))) : consume(std::forward<Args>(args))));
            return *this;
        };

        //seek
        pos_type tellp() {
            if (this->fail())return pos_type{ -1 };
            else return src->pubseekoff(0, ios::cur, ios::out);
        };
        io_traits& seekp(pos_type pos) {
            if (!this->fail()) src->pubseekpos(pos, ios::out);
            else this->setstate(ios::failbit, "io_traits::seekp: fail to seek");
            return *this;
        };
        io_traits& seekp(off_type off, ios::seekdir dir) {
            if (!this->fail()) src->pubseekoff(off, dir, ios::out);
            else this->setstate(ios::failbit, "io_traits::seekp: fail to seek");
            return *this;
        }

        //operator

        //for the type that specialized the parser
        template<typename Type>
        io_traits& operator<<(Type&& value) {
            parser<Type, char_type>::parse(std::forward<Type>(value), this, src);
            return *this;
        };
        io_traits& operator<<(const char_type value) {
            if (src->sputc(value) == traits_type::eof()) {
                this->setstate(ios::badbit, "io_traits: no such memory");
            }
            else {
                this->clear();
            }
            return *this;
        };
        io_traits& operator<<(const char_type* value) {
            if (src->sputn(value, traits_type::length(value)) != traits_type::length(value)) {
                this->setstate(ios::badbit, "io_traits: value too large");
            }
            else {
                this->clear();
            }
            return *this;
        };
        template<std::size_t N>
        io_traits& operator<<(const char_type(&value)[N]) {
            if (src->sputn(value, N) != N) {
                this->setstate(ios::badbit, "io_traits: value too large");
            }
            else {
                this->clear();
            }
            return *this;
        };
        template<typename Alloc>
        io_traits& operator<<(const std::basic_string<CharT, Traits, Alloc>& value) {
            if (src->sputn(value.c_str(), value.size()) != value.size()) {
                this->setstate(ios::badbit, "io_traits: value too large");
            }
            else {
                this->clear();
            }
            return *this;
        };
        io_traits& operator<<(std::basic_string_view<CharT, Traits> value) {
            if (src->sputn(value.data(), value.size()) != value.size()) {
                this->setstate(ios::badbit, "io_traits: value too large");
            }
            else {
                this->clear();
            }
            return *this;
        };
        io_traits& operator<<(ios_traits& (*func)(ios_traits&)) {
            func(*this);
            return *this;
        };
        io_traits& operator<<(ios_traits& (&func)(ios_traits&)) {
            func(*this);
            return *this;
        };
        //for the type that specialized the scanner
        template<typename Type>
        io_traits& operator>>(Type& value) {
            if ((this->flag() & fmtflag::skipws) != fmtflag{})this->skipws();
            scanner<Type, char_type>::scan(value, this, src);
            return *this;
        };
        template<std::size_t N>
        io_traits& operator>>(const char_type(&value)[N]) {
            if ((this->flag() & fmtflag::skipws) != fmtflag{})this->skipws();
            scanner<char_type, char_type>::scan(value, this, src);
            return *this;
        };
        template<typename Trait, typename SAlloc>
        io_traits& operator>>(std::basic_string<char_type, Trait, SAlloc>& value) {
            if ((this->flag() & fmtflag::skipws) != fmtflag{})this->skipws();
            scanner<char_type, char_type>::scan(value, this, src);
        };
        io_traits& operator>>(std::istringstream& iss) {
            if ((this->flag() & fmtflag::skipws) != fmtflag{})this->skipws();
            if ((this->flag() & fmtflag::stdcpy) == fmtflag{}) {
                iss.str(src->view());
            }
            else {
                StringView view = src->view();
                view.remove_prefix(src->gptr() - src->eback());
                iss.str(view);
            }
            return *this;
        };
        //C-type I/O support
        io_traits& operator>>(std::FILE* dest) {
            if ((this->flag() & fmtflag::skipws) != fmtflag{})this->skipws();
            src->underflow();
            if ((this->flag() & fmtflag::stdcpy) != fmtflag{}) {
                std::fwrite(src->eback(), sizeof(char_type), src->egptr() - src->eback(), dest);
            }
            else {
                std::fwrite(src->gptr(), sizeof(char_type), src->egptr() - src->gptr(), dest);
                src->gbump(src->egptr() - src->gptr());
            }
            return *this;
        };
        io_traits& operator>>(ios_traits& (*func)(ios_traits&))noexcept {
            func(*this);
            return *this;
        };
        io_traits& operator>>(ios_traits& (&func)(ios_traits&))noexcept {
            func(*this);
            return *this;
        };
    };

    template<typename CharT,
        typename Derived,
        typename Traits = std::char_traits<CharT>,
        typename... used_default_func
    >class __EMP_BASE basic_streambuf :protected used_default_func... {
    protected:
        using gptr_t = CharT*;
        using pptr_t = CharT*;
        using traits_type = Traits;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;
        using int_type = typename traits_type::int_type;
        using char_type = CharT;
        using streamsize = std::streamsize;
        inline static constexpr int_type eof = traits_type::eof();
        using _To = Derived;

        friend Derived;

        friend xsputn_f;
        friend xsgetn_f;
        friend xoverflow_f;
        friend xunderflow_f;
        friend xuflow_f;
        friend xpbackfail_f;
        friend setbuf_f;
        friend sync_f;
        friend seekoff_f;
        friend seekpos_f;
        friend showmanyc_f;
    private:
        gptr_t _g_first;//end of putback position
        gptr_t _g_nptr;//get pointer
        gptr_t _g_eptr;//end get position

        pptr_t _p_first;//begin get area
        pptr_t _p_nptr;//put pointer
        pptr_t _p_eptr;//end put position
    protected:

        /*get series function*/
        inline char_type* eback()const noexcept { return _g_first; };
        inline char_type* gptr()const noexcept { return _g_nptr; };
        inline char_type* egptr()const noexcept { return _g_eptr; };

        inline void gbump(int count) { _g_nptr += count; };
        inline void setg(char_type* gbegin, char_type* gcurrent, char_type* gend) {
            _g_first = gbegin;
            _g_nptr = gcurrent;
            _g_eptr = gend;
        };

        /*put series function*/
        inline char_type* pbase()const noexcept { return _p_first; };
        inline char_type* pptr()const noexcept { return _p_nptr; };
        inline char_type* epptr()const noexcept { return _p_eptr; };

        inline void pbump(int count) { _p_nptr += count; };
        inline void setp(char_type* pbegin, char_type* pend) {
            _p_first = pbegin;
            _p_nptr = pbegin;
            _p_eptr = pend;
        };

        inline int_type showmanyc()noexcept(_xshowmanyc<used_default_func...> || (!_xshowmanyc<used_default_func...> && noexcept(static_cast<_To*>(this)->xshowmanyc()))) {
            if constexpr (_xshowmanyc<used_default_func...>) {
                return showmanyc_f::xshowmanyc(this);
            }
            else {
                return static_cast<_To*>(this)->xshowmanyc();
            }
        };

        inline int_type overflow(int_type ch = traits_type::eof())noexcept(_xoverflow<used_default_func...> ||
            (!_xoverflow<used_default_func...> && noexcept(static_cast<_To*>(this)->xoverflow(ch)))) {
            if constexpr (_xoverflow<used_default_func...>) {
                return xoverflow_f::xoverflow(this, ch);
            }
            else {
                return static_cast<_To*>(this)->xoverflow(ch);
            }
        };

        inline int_type underflow()noexcept(_xunderflow<used_default_func...> ||
            (!_xunderflow<used_default_func...> && noexcept(static_cast<_To*>(this)->xunderflow()))) {
            if constexpr (_xunderflow<used_default_func...>) {
                return xunderflow_f::xunderflow(this);
            }
            else {
                return static_cast<_To*>(this)->xunderflow();
            }
        };
        inline int_type uflow()noexcept(_xuflow<used_default_func...> ||
            (!_xuflow<used_default_func...> && noexcept(static_cast<_To*>(this)->xuflow()))) {
            if constexpr (_xuflow<used_default_func...>) {
                return xuflow_f::xuflow(this);
            }
            else {
                return static_cast<_To*>(this)->xuflow();
            }
        };

        inline int_type pbackfail(int_type c = traits_type::eof())noexcept(_xpbackfail<used_default_func...> ||
            (!_xpbackfail<used_default_func...> && noexcept(static_cast<_To*>(this)->xpbackfail(c)))) {
            if constexpr (_xpbackfail<used_default_func...>) {
                return xpbackfail_f::xpbackfail(this, c);
            }
            else {
                return static_cast<_To*>(this)->xpbackfail(c);
            }
        };

        basic_streambuf()noexcept :_g_first(nullptr), _g_nptr(nullptr), _g_eptr(nullptr),
            _p_first(nullptr), _p_nptr(nullptr), _p_eptr(nullptr) {
        };
        basic_streambuf(const basic_streambuf& other) :
            _g_first(other._g_first), _g_nptr(other._g_nptr), _g_eptr(other._g_eptr),
            _p_eptr(other._p_eptr), _p_nptr(other._p_nptr), _p_first(other._p_first) {
        };
        basic_streambuf& operator=(const basic_streambuf& other) {
            _g_first = other._g_first;
            _g_nptr = other._g_nptr;
            _g_eptr = other._g_eptr;
            _p_first = other._p_first;
            _p_nptr = other._p_nptr;
            _p_eptr = other._p_eptr;
        };
        void swap(basic_streambuf& other)noexcept(
            noexcept(std::swap(_g_first, other._g_first)),
            noexcept(std::swap(_g_nptr, other._g_nptr)),
            noexcept(std::swap(_g_eptr, other._g_eptr)),
            noexcept(std::swap(_p_first, other._p_first)),
            noexcept(std::swap(_p_nptr, other._p_nptr)),
            noexcept(std::swap(_p_eptr, other._p_eptr))) {
            std::swap(_g_first, other._g_first);
            std::swap(_g_nptr, other._g_nptr);
            std::swap(_g_eptr, other._g_eptr);
            std::swap(_p_first, other._p_first);
            std::swap(_p_nptr, other._p_nptr);
            std::swap(_p_eptr, other._p_eptr);
        };
        ~basic_streambuf() = default;
    public:
        /*put character function*/
        inline int_type sputc(char_type c)noexcept(noexcept(overflow(traits_type::to_int_type(c)))) {
            if (_p_nptr == _p_eptr) return static_cast<_To*>(this)->overflow(traits_type::to_int_type(c));
            *_p_nptr++ = c;
            return traits_type::to_int_type(c);
        };
        inline streamsize sputn(const char_type* s, streamsize num)noexcept((_xsputn<used_default_func...>) ||
            (!_xsputn<used_default_func...> && noexcept(static_cast<_To*>(this)->xsputn(s, num)))) {
            if constexpr (_xsputn<used_default_func...>) {
                return xsputn_f::xsputn(this, s, num);
            }
            else {
                return static_cast<_To*>(this)->xsputn(s, num);
            }
        };
        /*get character function*/

        inline int_type sgetc()noexcept(noexcept(underflow())) {
            if (_g_nptr == _g_eptr)return underflow();
            return traits_type::to_int_type(*_g_nptr);
        };
        inline streamsize sgetn(char_type* s, streamsize num)noexcept(!_xsgetn<used_default_func...> && noexcept(static_cast<_To*>(this)->xsgetn(s, num))) {
            if constexpr (_xsgetn<used_default_func...>) {
                return xsgetn_f::xsgetn(this, s, num);
            }
            else {
                return static_cast<_To*>(this)->xsgetn(s, num);
            }
        };
        inline int_type sbumpc()noexcept(noexcept(uflow())) {
            if (_g_nptr == _g_eptr)return uflow();
            return traits_type::to_int_type(*_g_nptr++);
        };
        inline int_type snextc()noexcept(noexcept(uflow()) && noexcept(sbumpc()) && noexcept(sgetc())) {
            if (sbumpc() == traits_type::eof())return traits_type::eof();
            return sgetc();
        };
        inline int_type sputbackc(char_type c)noexcept(noexcept(pbackfail(traits_type::to_int_type(c)))) {
            if (_g_nptr == _g_first || !traits_type::eq(c, _g_nptr[-1]))return pbackfail(traits_type::to_int_type(c));
            return traits_type::to_int_type(*--_g_nptr);
        };
        inline int_type sungetc()noexcept(noexcept(pbackfail())) {
            if (_g_nptr == _g_first)return pbackfail();
            return traits_type::to_int_type(*--_g_nptr);
        };
        inline streamsize in_avail()noexcept(noexcept(showmanyc())) {
            if (_g_nptr < _g_eptr) return static_cast<streamsize>(_g_eptr - _g_nptr);
            return showmanyc();
        };

        /*pub series function*/
        inline int pubsync()noexcept((_sync<used_default_func...>) ||
            (!_sync<used_default_func...> && noexcept(static_cast<_To*>(this)->sync()))) {
            if constexpr (_sync<used_default_func...>) {
                return sync_f::sync(this);
            }
            else {
                return static_cast<_To*>(this)->sync();
            }
        };
        inline auto pubsetbuf(char_type* s, streamsize size)noexcept(_setbuf<used_default_func...> ||
            (!_setbuf<used_default_func...> && noexcept(static_cast<_To*>(this)->setbuf(s, size)))) {
            if constexpr (_setbuf<used_default_func...>) {
                return setbuf_f::setbuf(this, s, size);
            }
            else {
                return static_cast<_To*>(this)->setbuf(s, size);
            }
        };
        inline pos_type pubseekoff(off_type pos, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)noexcept(_seekoff<used_default_func...> ||
            (!_seekoff<used_default_func...> && noexcept(static_cast<_To*>(this)->seekoff(pos, dir, which)))) {
            if constexpr (_seekoff<used_default_func...>) {
                return seekoff_f::seekoff(this, pos, dir, which);
            }
            else {
                return static_cast<_To*>(this)->seekoff(pos, dir, which);
            }
        };
        inline pos_type pubseekpos(pos_type pos, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)noexcept(_seekpos<used_default_func...> ||
            (!_seekpos<used_default_func...> && noexcept(static_cast<_To*>(this)->seekpos(pos, which)))) {
            if constexpr (_seekpos<used_default_func...>) {
                return seekpos_f::seekpos(this, pos, which);
            }
            else {
                return static_cast<_To*>(this)->seekpos(pos, which);
            }
        };

    };

    constexpr std::streamsize get_buf_size(std::streamsize size)noexcept {
        std::streamsize _size{ 1 };
        for (; _size < size;) _size <<= 1;
        return _size;
    };

    template<typename CharT,
        typename Traits = std::char_traits<CharT>,
        typename Alloc = std::allocator<CharT>
    >class basic_stringbuf :public basic_streambuf<CharT, basic_stringbuf<CharT, Traits, Alloc>, Traits,
        sync_t, xsgetn_t, xshowmanyc_t, xuflow_t> {
    private:
        /*typedef*/
        using char_type = CharT;
        using allocator_type = Alloc;
        using traits_type = Traits;
        using String = std::basic_string<CharT, Traits, Alloc>;
        using StringView = std::basic_string_view<CharT, Traits>;
        using ios = std::ios_base;

        using int_type = typename traits_type::int_type;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;

        using streamsize = std::streamsize;

        /*friend*/
        template<typename, typename, typename, typename...>
        friend class basic_streambuf;
        template<typename, typename, typename>
        friend class input_traits;
        template<typename, typename, typename>
        friend class output_traits;
        template<typename, typename, typename>
        friend class io_traits;

        template<typename, typename, typename>
        friend struct parser;
        template<typename, typename, typename>
        friend struct scanner;

        /*data member*/
        allocator_type allocator;
        char_type* _buf;
        std::ios_base::openmode mode;
        bool from_out_str;
        std::uint64_t current_size;
        std::array<char_type, 32> _heap_buf;

        /*reallocate*/
        //default reallocate strategy, alway reallocate double size
        bool default_reallocate() {
            if (from_out_str)return false;
            streamsize _n_size = current_size * 2;
            auto _p_dis = this->pptr() - _buf;
            auto _g_dis = this->gptr() - _buf;
            try {
                char_type* _n = allocator.allocate(_n_size);
                std::memcpy(_n, _buf, _p_dis);
                if (_buf != _heap_buf.data()) allocator.deallocate(_buf, current_size);
                _buf = _n;
            }
            catch (...) {
                return false;
            }
            current_size = _n_size;
            this->setp(_buf, _buf + _n_size);
            this->pbump(_p_dis);
            this->setg(_buf, _buf + _g_dis, this->pptr());
            return true;
        };

        //increase buffer size, copy old data from buffer, if used custom buffer, it won't reallocate it
        bool incremental_reallocate(streamsize size) {
            if (from_out_str)return false;
            streamsize _n_size = get_buf_size(current_size + size);
            auto _p_dis = this->pptr() - _buf;
            auto _g_dis = this->gptr() - _buf;
            try {
                char_type* _n = allocator.allocate(_n_size);
                std::memcpy(_n, _buf, _p_dis);
                if (_buf != _heap_buf.data()) allocator.deallocate(_buf, current_size);
                _buf = _n;
            }
            catch (...) {
                return false;
            }
            current_size = _n_size;
            this->setp(_buf, _buf + _n_size);
            this->pbump(_p_dis);
            this->setg(_buf, _buf + _g_dis, this->pptr());
            return true;
        };

        //replace old buffer, ignore whether use custom buffer or not(if yes, won't deallocate buffer)
        //btw, it will clear the old data
        bool replace_reallocate(streamsize size) {
            if (size <= 32) {
                if (_buf != _heap_buf.data() && !from_out_str) allocator.deallocate(_buf, current_size);
                _buf = _heap_buf.data();
                this->setp(_buf, _buf + 32);
                this->setg(_buf, _buf, _buf);
            }
            else {
                try {
                    char_type* _n = allocator.allocate(size);
                    if (_buf != _heap_buf.data() && !from_out_str) allocator.deallocate(_buf, current_size);
                    _buf = _n;
                    this->setp(_buf, _buf + size);
                    this->setg(_buf, _buf, _buf);
                }
                catch (...) {
                    return false;
                }
            }
            current_size = size;
            from_out_str = false;
            return true;
        };

        /*init*/
        void _Init(size_t size) {
            if ((mode & ios::in | ios::out) == (ios::in | ios::out)) this->setg(_buf, _buf, _buf);
            else if (mode & ios::in)this->setg(_buf, _buf, _buf + size);
            else this->setg(nullptr, nullptr, nullptr);
            if (mode & ios::out)this->setp(_buf, _buf + size);
            else this->setp(nullptr, nullptr);
        };

        /*derived function*/

        //to be faster, if check the buffer to ensure it won't copy redundant data it put in before
        inline streamsize xsputn(const char_type* s, streamsize size) {
            if (this->epptr() - this->pptr() < size) incremental_reallocate(size);
            streamsize _n;
            if (this->epptr() - this->pptr() < size) {
                _n = this->epptr() - this->pptr();
            }
            else {
                _n = size;
            }
            std::memcpy(this->pptr(), s, sizeof(char_type) * size);
            this->pbump(_n);
            return _n;
        };

        int_type xoverflow(int_type c = traits_type::eof()) {
            if (!default_reallocate())return traits_type::eof();
            if (traits_type::eq_int_type(c, traits_type::eof())) return traits_type::not_eof(c);
            this->sputc(traits_type::to_char_type(c));
            return c;
        };

        //it do not follow C++ standard requirement, it will align buffer if it has more data first
        //because it need to support flush and parser<basic_ostream>
        int_type xunderflow() {
            if (this->pptr() > this->egptr()) {
                this->setg(_buf, this->gptr(), this->pptr());
                return traits_type::to_int_type(*(this->gptr()));
            }
            else if (this->egptr() > this->gptr()) {
                return traits_type::to_int_type(*(this->gptr()));
            }
            else {
                return traits_type::eof();
            }
        };

        auto setbuf(char_type* s, streamsize size) {
            if (size == 0)return this;
            from_out_str = true;
            this->setg(s, s, s);
            this->setp(s, s + size);
            return this;
        };

        pos_type seekoff(off_type off, ios::seekdir dir, ios::openmode which = ios::in | ios::out) {
            if ((which & (ios::in | ios::out)) == (ios::in | ios::out) &&
                dir & ios::cur) return off_type{ -1 };
            else if ((which & ios::out) != (ios::out))return off_type{ -1 };
            else if ((which & ios::in) != (ios::in))return off_type{ -1 };
            off_type new_off{};
            switch (dir) {
            case ios::beg:
                new_off = 0;
                break;
            case ios::cur:
                if ((which & ios::in) == ios::in) new_off = this->gptr() - this->eback();
                else new_off = this->pptr() - this->pbase();
                break;
            case ios::end:
                new_off = this->pptr() - _buf;
            [[unlikely]] default:
                return off_type{ -1 };
            }
            new_off += off;
            if (new_off < 0 || this->pptr() - _buf < new_off)return off_type{ -1 };
            if (new_off != 0) {
                if ((which & ios::in) && !this->gptr())return off_type{ -1 };
                if ((which & ios::out) && !this->pptr())return off_type{ -1 };
            }
            if (which & ios::in) {
                this->setg(this->eback(), this->eback() + new_off, this->pptr());
            }
            if (which & ios::out) {
                this->setp(this->pbase(), this->epptr());
                this->pbump(new_off);
            }
            return new_off;
        };

        pos_type seekpos(pos_type sp, ios::openmode which = ios::in | ios::out) {
            if (seekoff(off_type{ sp }, ios::beg, which) != pos_type{ off_type{-1} })return sp;
            return off_type{ -1 };
        };

        int_type xpbackfail(int_type c = traits_type::eof()) {
            if (this->eback() < this->gptr()) {
                if (traits_type::eq_int_type(c, traits_type::eof())) {
                    this->setg(this->eback(), this->gptr() - 1, this->egptr());
                    return traits_type::not_eof(c);
                }
                if ((mode & ios::out) || traits_type::eq(traits_type::to_char_type(c), (this->gptr())[-1])) {
                    this->setg(this->eback(), this->gptr() - 1, this->pptr());
                    *(this->gptr()) = traits_type::to_char_type(c);
                    return c;
                }
            }
            return traits_type::eof();
        };

    public:
        explicit basic_stringbuf(std::ios_base::openmode which) :basic_streambuf<CharT, basic_stringbuf<CharT, Traits, Alloc>, Traits, sync_t, xsgetn_t, xshowmanyc_t, xuflow_t>{},
            allocator{}, _buf{ _heap_buf.data() }, mode{ which }, from_out_str{ false }, current_size{ 32 }, _heap_buf{}{
            _Init(32);
        };
        basic_stringbuf() :basic_stringbuf(ios::in | ios::out) {};
        explicit basic_stringbuf(String& str, ios::openmode which = ios::in | ios::out) :basic_streambuf<CharT, basic_stringbuf<CharT, Traits, Alloc>, Traits, sync_t, xsgetn_t, xshowmanyc_t, xuflow_t>{},
            allocator{}, _buf{ str.data() }, mode{ which }, from_out_str{ true }, current_size{ str.size() }, _heap_buf{} {
            _Init(str.size());
        };
        basic_stringbuf(ios::openmode which, const Alloc& a) :basic_streambuf<CharT, basic_stringbuf<CharT, Traits, Alloc>, Traits, sync_t, xsgetn_t, xshowmanyc_t, xuflow_t>{},
            allocator{ a }, _buf{ str.data() }, mode{ which }, from_out_str{ true }, current_size{ 32 }, _heap_buf{} {};
        explicit basic_stringbuf(Alloc& a) :basic_stringbuf(ios::in | ios::out, a) {};
        template<typename SAlloc>
        explicit basic_stringbuf(std::basic_string<CharT, Traits, SAlloc>& str, ios::openmode which = ios::in | ios::out) :basic_streambuf<CharT, basic_stringbuf<CharT, Traits, Alloc>, Traits, sync_t, xsgetn_t, xshowmanyc_t, xuflow_t>{},
            allocator{}, _buf{ str.data() }, mode{ which }, from_out_str{ true }, current_size{ str.size() }, _heap_buf{} {
            _Init(str.size());
        };
        template<typename SAlloc>
        basic_stringbuf(std::basic_string<CharT, Traits, SAlloc>& str, ios::openmode which, const Alloc& a) :basic_streambuf<CharT, basic_stringbuf<CharT, Traits, Alloc>, Traits, sync_t, xsgetn_t, xshowmanyc_t, xuflow_t>{},
            allocator{ a }, _buf{ str.data() }, mode{ which }, from_out_str{ true }, current_size{ str.size() }, _heap_buf{}{
            _Init(str.size());
        };
        template<typename SAlloc>
        basic_stringbuf(std::basic_string<CharT, Traits, SAlloc>& str, const Alloc& a) :basic_stringbuf(str, ios::in | ios::out, a) {};
        basic_stringbuf(basic_stringbuf&& other) :basic_streambuf<CharT, basic_stringbuf<CharT, Traits, Alloc>, Traits, sync_t, xsgetn_t, xshowmanyc_t, xuflow_t>{},
            allocator{ std::move(other.allocator) }, mode{ std::move(other.mode) }, from_out_str{ std::move(other.from_out_str) }, current_size{ std::move(other.current_size) }, _heap_buf{ std::move(other._heap_buf) }{
            if (this != &other) {
                _buf = other._buf;
                other._buf = nullptr;
                setg(other.eback(), other.gptr(), other.egptr());
                other.setg(nullptr, nullptr, nullptr);
                setp(other.pptr(), other.epptr());
                other.setp(nullptr, nullptr);
            }
        };

        basic_stringbuf(const basic_stringbuf&) = delete;
        basic_stringbuf& operator=(basic_stringbuf&& rhs) {
            if (this != &rhs) {
                allocator = std::move(rhs.allocator);
                mode = std::move(rhs.mode);
                from_out_str = std::move(rhs.from_out_str);
                _buf = rhs._buf;
                rhs._buf = nullptr;
                setg(rhs.eback(), rhs.gptr(), rhs.egptr());
                rhs.setg(nullptr, nullptr, nullptr);
                setp(rhs.pptr(), rhs.epptr());
                rhs.setp(nullptr, nullptr);
            }
        };

        /*public function*/

        String str()const {
            if ((mode & (ios::in | ios::out)) == (ios::in | ios::out) || (mode & ios::out)) {
                return{ this->pbase(),this->pptr() };
            }
            else {
                return{ this->eback(),this->egptr() };
            }
        };
        void str(const String& s) {
            ptrdiff_t dis = this->epptr() - this->pbase();
            if (s.size() > dis)incremental_reallocate(s.size());
            traits_type::copy(_buf, s.c_str(), s.size());
            this->setg(_buf, _buf, _buf + s.size());
            this->setp(_buf, this->epptr());
        };
        template<typename SAlloc>
        void str(const std::basic_string<CharT, Traits, SAlloc>& s) {
            ptrdiff_t dis = this->epptr() - this->pbase();
            if (s.size() > dis)incremental_reallocate(s.size());
            traits_type::copy(_buf, s.c_str(), s.size());
            this->setg(_buf, _buf, _buf + s.size());
            this->setp(_buf, this->epptr());
        };
        void str(const char_type* s) {
            ptrdiff_t dis = this->epptr() - this->pbase();
            if (traits_type::length(s) > dis)incremental_reallocate(traits_type::length(s));
            traits_type::copy(_buf, s, traits_type::length(s));
            this->setg(_buf, _buf, _buf + traits_type::length(s));
            this->setp(_buf, this->epptr());
        }
        void str(StringView s) {
            ptrdiff_t dis = this->epptr() - this->pbase();
            if (s.size() > dis)incremental_reallocate(s.size());
            traits_type::copy(_buf, s.data(), s.size());
            this->setg(_buf, _buf, _buf + s.size());
            this->setp(_buf, this->epptr());
        };

        StringView view()const noexcept {
            if ((mode & (ios::in | ios::out)) == (ios::in | ios::out) || (mode & ios::out)) {
                return { this->pbase(), static_cast<std::size_t>(this->pptr() - this->pbase()) };
            }
            else if (mode & ios::in) {
                return { this->eback(), static_cast<std::size_t>(this->egptr() - this->eback()) };
            }
            else {
                return {};
            }
        };

        auto reset_buf(streamsize size) {
            if (size == 0)return this;
            replace_reallocate(size);
            return this;
        }

        inline allocator_type get_allocator()const noexcept { return allocator; };

        void swap(basic_stringbuf& rhs)noexcept(std::allocator_traits<Alloc>::propagate_on_container_swap::value ||
            std::allocator_traits<Alloc>::is_always_equal::value) {
            basic_streambuf::swap(rhs);
            std::swap(_buf, rhs._buf);
            std::swap(current_size, rhs.current_size);
            std::swap(allocator, rhs.allocator);
            std::swap(from_out_str, rhs.from_out_str);
            std::swap(mode, rhs.mode);
            std::swap(_heap_buf, rhs._heap_buf);
        };

        ~basic_stringbuf() {
            if (_buf && _buf != _heap_buf.data() && !from_out_str) allocator.deallocate(_buf, current_size);
        };
    };

    template<typename CharT, typename Traits = std::char_traits<CharT>, typename Alloc = std::allocator<CharT>>
    class basic_ostringstream :public output_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits> {
    private:
        using buf_type = basic_stringbuf<CharT, Traits, Alloc>;
        using char_type = CharT;
        using traits_type = Traits;
        using ios = std::ios_base;
        using String = std::basic_string<CharT, Traits, Alloc>;
        using StringView = std::basic_string_view<CharT, Traits>;
        buf_type _buf;

        template<typename, typename, typename>
        friend struct parser;
    public:

        explicit basic_ostringstream(ios::openmode mode) :_buf(mode), output_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits>(&_buf) {};
        basic_ostringstream() :basic_ostringstream(ios::out) {};
        explicit basic_ostringstream(const String& str, ios::openmode mode) :_buf(str, mode), output_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits>(&_buf) {};
        basic_ostringstream(ios::openmode mode, const Alloc& a) :_buf(mode, a), output_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits>(&_buf) {};
        template<typename SAlloc>
        basic_ostringstream(const std::basic_string<CharT, Traits, SAlloc>& str, ios::openmode mode, const Alloc& a) :_buf(str, mode, a), output_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits>(&_buf) {};
        template<typename SAlloc>
        basic_ostringstream(const std::basic_string<CharT, Traits, SAlloc>& str, const Alloc& a) :basic_ostringstream(str, ios::out, a) {};
        basic_ostringstream(basic_ostringstream&& other) :_buf(std::move(other._buf)), output_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits>(std::move(other)) {};

        void swap(basic_ostringstream& other)noexcept {
            output_traits::swap(other);
            _buf.swap(other._buf);
        };

        inline buf_type* rdbuf()const { return &_buf; };
        inline buf_type* rdbuf(char_type* sb, std::streamsize size) { return _buf.pubsetbuf(sb, size); };

        inline buf_type* reset(std::streamsize size = 32) { return _buf.reset_buf(size); };

        inline String str()const& { return _buf.str(); };
        inline String str()&& { return std::move(_buf.str()); };
        inline void str(const String& s) {
            _buf.str(s);
            this->clear();
        };
        inline void str(StringView sv) {
            _buf.str(sv);
            this->clear();
        };
        inline void str(const char_type* s) {
            _buf.str(s);
            this->clear();
        };

        StringView view()const noexcept { return _buf.view(); };

    };
    template<typename CharT,
        typename Traits = std::char_traits<CharT>,
        typename Allocator = std::allocator<CharT>
    >class basic_istringstream :public input_traits<basic_stringbuf<CharT, Traits, Allocator>, CharT, Traits> {
    private:
        using buf_type = basic_stringbuf<CharT, Traits, Allocator>;
        using String = std::basic_string<CharT, Traits, Allocator>;
        using StringView = std::basic_string_view<CharT, Traits>;
        using traits_type = Traits;
        using char_type = CharT;
        using ios = std::ios_base;
        buf_type _buf;

        template<typename, typename, typename>
        friend struct scanner;
    public:

        explicit basic_istringstream(ios::openmode mode) :_buf(mode), input_traits<basic_stringbuf<CharT, Traits, Allocator>, CharT, Traits>(&_buf) {};
        basic_istringstream() :basic_istringstream(ios::in) {};
        explicit basic_istringstream(String& str, ios::openmode mode = ios::in) :_buf(str, mode), input_traits<basic_stringbuf<CharT, Traits, Allocator>, CharT, Traits>(&_buf) {};
        basic_istringstream(ios::openmode mode, const Allocator& alloc) :_buf(mode, alloc), input_traits<basic_stringbuf<CharT, Traits, Allocator>, CharT, Traits>(&_buf) {};
        template<typename SAlloc>
        basic_istringstream(std::basic_string<CharT, Traits, SAlloc>& str, ios::openmode mode, const Allocator& a) :_buf(str, mode, a), input_traits<basic_stringbuf<CharT, Traits, Allocator>, CharT, Traits>(&_buf) {};
        template<typename SAlloc>
        basic_istringstream(std::basic_string<CharT, Traits, SAlloc>& str, const Allocator& a) :basic_istringstream(str, ios::in, a) {};
        template<typename SAlloc>
        explicit basic_istringstream(std::basic_string<CharT, Traits, SAlloc>& str, ios::openmode mode = ios::in) :_buf(str, mode), input_traits<basic_stringbuf<CharT, Traits, Allocator>, CharT, Traits>(&_buf) {};
        basic_istringstream(basic_istringstream&& other) :_buf(std::move(other._buf)), input_traits<basic_stringbuf<CharT, Traits, Allocator>, CharT, Traits>(std::move(other)) {};

        void swap(basic_istringstream& other)noexcept {
            input_traits::swap(other);
            _buf.swap(other._buf);
        };

        inline buf_type* rdbuf()const { return &_buf; };
        inline buf_type* rdbuf(char_type* sb, std::streamsize size) { return _buf.pubsetbuf(sb, size); };

        inline buf_type* reset(std::streamsize size = 32) { return _buf.reset_buf(size); };

        inline String str()const& { return _buf.str(); };
        inline String str()&& { return std::move(_buf.str()); };
        inline void str(const String& s) {
            _buf.str(s);
            this->clear();
        };
        template<typename SAlloc>
        inline void str(const std::basic_string<CharT, Traits, SAlloc>& s) {
            _buf.str(s);
            this->clear();
        };
        inline void str(const char_type* s) {
            _buf.str(s);
            this->clear();
        };
        inline void str(StringView sv) {
            _buf.str(sv);
            this->clear();
        };

        inline StringView view() const noexcept { return _buf.view(); };

        ~basic_istringstream() = default;
    };

    template<typename CharT, typename Traits = std::char_traits<CharT>, typename Alloc = std::allocator<CharT>>
    class basic_stringstream :public io_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits> {
    private:
        using buf_type = basic_stringbuf<CharT, Traits, Alloc>;
        using char_type = CharT;
        using traits_type = Traits;
        using pos_type = typename traits_type::pos_type;
        using off_type = typename traits_type::off_type;
        using ios = std::ios_base;
        using String = std::basic_string<CharT, Traits, Alloc>;
        using StringView = std::basic_string_view<CharT, Traits>;

        buf_type _buf;

        template<typename, typename, typename>
        friend struct parser;
        template<typename, typename, typename>
        friend struct scanner;
    public:
        explicit basic_stringstream(ios::openmode mode) :_buf(mode), io_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits>(&_buf) {};
        basic_stringstream() :basic_stringstream(ios::in | ios::out) {};
        explicit basic_stringstream(String& str, ios::openmode mode = ios::in | ios::out) :_buf(str, mode), io_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits>(&_buf) {};
        basic_stringstream(ios::openmode mode, const Alloc& a) :_buf(mode, a), io_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits>(&_buf) {};
        template<typename SAlloc>
        basic_stringstream(std::basic_string<CharT, Traits, SAlloc>& str, ios::openmode mode, const Alloc& a) :_buf(str, mode, a), io_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits>(&_buf) {};
        template<typename SAlloc>
        basic_stringstream(std::basic_string<CharT, Traits, SAlloc>& str, const Alloc& a) :basic_stringstream(str, ios::in | ios::out, a) {};
        template<typename SAlloc>
        basic_stringstream(std::basic_string<CharT, Traits, SAlloc>& str, ios::openmode mode = ios::in | ios::out) :_buf(str, mode), io_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits>(&_buf) {};
        basic_stringstream(basic_stringstream&& other) :_buf(std::move(other._buf)), io_traits<basic_stringbuf<CharT, Traits, Alloc>, CharT, Traits>(std::move(other)) {};

        void swap(basic_stringstream& other)noexcept(true) {
            std::swap(_buf, other._buf);
            io_traits::swap(other);
        };

        inline buf_type* rdbuf()const { return &_buf; };
        inline buf_type* rdbuf(char_type* sb, std::streamsize size) { return _buf.pubsetbuf(sb, size); };

        inline buf_type* reset(std::streamsize size = 32) { return _buf.reset_buf(size); };

        inline String str()const& { return _buf.str(); };
        inline String str()&& { return std::move(_buf.str()); };
        inline void str(const String& s) {
            _buf.str(s);
            this->clear();
        };
        template<typename SAlloc>
        inline void str(const std::basic_string<CharT, Traits, SAlloc>& s) {
            _buf.str(s);
            this->clear();
        };
        inline void str(const char_type* s) {
            _buf.str(s);
            this->clear();
        };
        inline void str(StringView s) {
            _buf.str(s);
            this->clear();
        };

        inline StringView view()const noexcept { return _buf.view(); };


        ~basic_stringstream() = default;
    };

    using ostringstream = basic_ostringstream<char>;
    using istringstream = basic_istringstream<char>;
    using stringstream = basic_stringstream<char>;
}// namespace fast_sstream
#endif //_FAST_SSTREAM_