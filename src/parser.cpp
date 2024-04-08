//
// Created by 白天宇 on 2024/4/3.
//
#include "iostream"
#include "string"
#include "../include/parser.h"
#include "cerrno"
#include "cstdlib"
#include "unordered_map"

using namespace json;

#define EXPECT(c, ch) do { assert(*c == (ch)); c++; } while(0)

void prints(std::string str) {
    std::cout << str << std::endl;
}

const std::unordered_map<std::string, Parser> &Parser::get_dict_() const {
    assert(type_ == L_OBJECT);
    return dict;
}

const double &Parser::get_num_() const {
    assert(type_ == L_NUMBER);
    return num_;
}

const lept_type &Parser::get_type_() const {
    return type_;
}

const std::string &Parser::get_string_() const {
    assert(type_ == L_STRING);
    return str_;
}

const std::vector<Parser> &Parser::get_arr_() const {
    assert(type_ == L_ARRAY);
    return arr_;
}

// "\"Hello\\nWorld\""  ====>  "Hello\nWorld"
void Parser::seeALLStr() {
    const char *cur_ptr = v_ptr;
    while (*cur_ptr != NULL) {
        std::cout << *cur_ptr;
        cur_ptr++;
    }
}

return_type Parser::parse_string() {
    std::string tmp;
    return_type res = parse_string_raw(tmp);
    if (res == L_PARSE_OK) {
        type_ = L_STRING;
        str_ = tmp;
    }
    return res;
}


return_type Parser::parse_string_raw(std::string &tmp) {
    EXPECT(v_ptr, '\"');
    const char *cur_ptr = v_ptr;
    unsigned u = 0, u2 = 0;
    while (*cur_ptr != '\"') {
        // 没解析到右边的引号，但是解析到了字符串结尾，说明这个字符串是不完整的
        if (*cur_ptr == '\0') {
            type_ = L_None;
            return L_PARSE_MISS_QUOTATION_MARK;
        }
        // 处理 9 种转义字符：当前字符是'\'，然后跳到下一个字符:"
        if (*cur_ptr == '\\' && ++cur_ptr) {
            // 先看当前字符，例如“号
            switch (*cur_ptr++) {
                case '\"':
                    tmp += '\"';
                    break;
                case '\\':
                    tmp += '\\';
                    break;
                case '/' :
                    tmp += '/';
                    break;
                case 'b' :
                    tmp += '\b';
                    break;
                case 'f' :
                    tmp += '\f';
                    break;
                case 'n' :
                    tmp += '\n';
                    break;
                case 'r' :
                    tmp += '\r';
                    break;
                case 't' :
                    tmp += '\t';
                    break;
                case 'u':
                    parse_hex4(cur_ptr, u);
                    // std::cout << *cur_ptr << *++cur_ptr << std::endl;
                    // std::cout << std::hex << u << std::endl;
                    // 高代理和低代理
                    if (u >= 0xD800 && u <= 0xDBFF) {
                        // 不为\符号
                        if (*cur_ptr++ != '\\') {
                            return L_PARSE_INVALID_UNICODE_SURROGATE;
                        }
                        // \的下一个不为u，注意上面的cur_ptr已经++了，移动到了u
                        if (*cur_ptr++ != 'u') {
                            return L_PARSE_INVALID_UNICODE_SURROGATE;
                        }
                        parse_hex4(cur_ptr, u2);
                        // 高代理项代码点（High-Surrogate Code Point）：在 U+D800 到 U+DBFF 范围内的 Unicode 代码点。
                        // 低代理项代码点（Low-Surrogate Code Point）：在 U+DC00 到 U+DFFF 范围内的 Unicode 代码点。
                        // 低代理项需要满足在这两个区间内
                        if (u2 < 0xDC00 || u2 > 0xDFFF) {
                            return L_PARSE_INVALID_UNICODE_SURROGATE;
                        }
                        // codepoint = 0x10000 + (H − 0xD800) × 0x400 + (L − 0xDC00)
                        // 0x400 = 1024，向左移动一位乘2
                        u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
                    }
                    parse_encode_utf8(tmp, u);
                    break;
                default:
                    return L_PARSE_INVALID_STRING_ESCAPE;
            }
        } else if ((unsigned char) *cur_ptr < 0x20) {
            return L_PARSE_INVALID_STRING_CHAR;
        } else {
            tmp += *cur_ptr++;
        }
    }
    // 更新字符串的位置,换到下一个点
    v_ptr = ++cur_ptr;
    return L_PARSE_OK;
}

//'A' - 10：这个表达式计算出字符 'A' 对应的十六进制数字的值。
// 由于在 ASCII 编码中，大写字母 A 的十进制值是 65，而十六进制的 A 对应的值是 10，所以 'A' - 10 的结果是 55，
// 即A的ASCII和十六进制之间的差值，所以ch也差这么多
// ch - ('A' - 10)：这个表达式将字符 ch 的 ASCII 值减去字符 'A' 的ASCII和十六进制的差值，
// 得到字符 ch 对应的十六进制数字的值。例如，如果 ch 是字符 'C'，它的 ASCII 值是 67，所以表达式的结果是 67 - 55 = 12。
// u |= ch - ('A' - 10)：这个表达式将转换后的整数值与变量 u 进行按位或操作，
// 并将结果赋值给 u。按位或操作的作用是将转换后的整数值的二进制位合并到 u 的对应位，形成最终的结果。
return_type Parser::parse_hex4(const char *&p, unsigned &u) {
    u = 0;
    for (size_t i = 0; i < 4; ++i) {
        char ch = *p++;
        u <<= 4;
        // 检查当前获取的字符 ch 是否为一个数字（0-9）。如果是数字，则将其转换为相应的整数值，并将其与 u 进行按位或操作。
        // 按位或操作的目的是将转换后的整数值合并到 u 中的相应位置。通过按位或操作，
        // 可以将转换后的整数值的二进制位设置为 1，从而与 u 的对应位进行合并，形成最终的结果。
        // 例如，假设 u 的初始值为 0x0，即二进制 0000。在第一次循环中，
        // 如果转换后的整数值为 2（二进制 0010），执行 u |= ch - '0' 会将 u 更新为 0x2（二进制 0010）。
        // 这样，在四次循环结束后，u 中将包含四个十六进制数字的值。
        if (isdigit(ch)) {
            u |= ch - '0';
        } else if (ch >= 'A' && ch <= 'F') {
            u |= ch - ('A' - 10);
        } else if (ch >= 'a' && ch <= 'f') {
            u |= ch - ('a' - 10);
        } else {
            return L_PARSE_INVALID_UNICODE_HEX;
        }
    }
}


// 需要看一下
void Parser::parse_encode_utf8(std::string &tmp, unsigned &u) {
    if (u <= 0x7F) {
        // 0x7F: 01111111
        // 0xFF: 11111111
        // 按位与，在该码点处的字节是这样0xxxxxxx，第一位为0
        // 譬如：
        // 0x6A: 1101010
        // 0xFF: 11111111
        // 前面补0然后按位与
        tmp += static_cast<char> (u & 0xFF);
    } else if (u <= 0x7FF) {
        tmp += static_cast<char> (0xC0 | ((u >> 6) & 0xFF));
        tmp += static_cast<char> (0x80 | (u & 0x3F));
    } else if (u <= 0xFFFF) {
        tmp += static_cast<char> (0xE0 | ((u >> 12) & 0xFF));
        tmp += static_cast<char> (0x80 | ((u >> 6) & 0x3F));
        tmp += static_cast<char> (0x80 | (u & 0x3F));
    } else {
        assert(u <= 0x10FFFF);
        tmp += static_cast<char> (0xF0 | ((u >> 18) & 0xFF));
        tmp += static_cast<char> (0x80 | ((u >> 12) & 0x3F));
        tmp += static_cast<char> (0x80 | ((u >> 6) & 0x3F));
        tmp += static_cast<char> (0x80 | (u & 0x3F));
    }
}


return_type Parser::parse(const std::string &str) {
    v_ptr = str.c_str();
    parse_whitespace();
    return_type res = parse_value();
    // 不合法的话直接跳出，否则继续去除后面的空格
    if (res != L_PARSE_OK) {
        return res;
    }
    parse_whitespace();
    // 去掉 Value 前后的空白，若 json 在一个值之后，空白之后还有其他字符的话，说明该 json 值是不合法的。
    if (*v_ptr != '\0') {
        type_ = L_None;
        return L_PARSE_ROOT_NOT_SINGULAR;
    }
    return res;
}

void Parser::parse_whitespace() {
    /* 过滤掉 json 字符串中的空白，即空格符、制表符、换行符、回车符 */
    while (*v_ptr == ' ' || *v_ptr == '\t' || *v_ptr == '\n' || *v_ptr == '\r')
        v_ptr++;
}


return_type Parser::parse_liter(const char *lit, lept_type cur_type) {
    EXPECT(v_ptr, lit[0]);
    size_t i;
    // lit到达末尾后
    for (i = 0; lit[i + 1] != '\0'; i++) {
        if (v_ptr[i] != lit[i + 1]) {
            type_ = L_None;
            return L_PARSE_INVALID_VALUE;
        }
    }
    v_ptr += i;
    type_ = cur_type;
    return L_PARSE_OK;
}

return_type Parser::parse_number() {
    const char *cur_ptr = v_ptr;
    if (*cur_ptr == '-')
        ++cur_ptr;
    if (*cur_ptr == '0')
        ++cur_ptr;
    else {
        if (!isdigit(*cur_ptr)) {
            type_ = L_None;
            return L_PARSE_INVALID_VALUE;
        }
        while (isdigit(*++cur_ptr));
    }
    if (*cur_ptr == '.') {
        if (!isdigit(*++cur_ptr)) {
            type_ = L_None;
            return L_PARSE_INVALID_VALUE;
        }
        while (isdigit(*++cur_ptr));
    }
    // 处理指数部分
    if (*cur_ptr == 'E' || *cur_ptr == 'e') {
        ++cur_ptr;
        if (*cur_ptr == '-' || *cur_ptr == '+')
            ++cur_ptr;
        if (!isdigit(*cur_ptr)) {
            type_ = L_None;
            return L_PARSE_INVALID_VALUE;
        }
        while (isdigit(*++cur_ptr));
    }

    errno = 0;
    double n = strtod(v_ptr, NULL);
    if (errno == ERANGE && (n == HUGE_VAL || n == -HUGE_VAL)) {
        type_ = L_None;
        return L_PARSE_NUMBER_TOO_BIG;
    }
    num_ = n;
    type_ = L_NUMBER;
    v_ptr = cur_ptr;
    return L_PARSE_OK;
}

return_type Parser::parse_value() {
    switch (*v_ptr) {
        case 't':
            return parse_liter("true", L_TRUE);
        case 'f':
            return parse_liter("false", L_FALSE);
        case 'n':
            return parse_liter("null", L_NULL);
        default:
            return parse_number();
        case '\"':
            return parse_string();
        case '[':
            return parse_array();
        case '{':
            return parse_object();
        case '\0':
            type_ = L_None;
            return L_PARSE_EXPECT_VALUE;
    }
}


return_type Parser::parse_array() {
    // Example:
    // [
    //        "Erich Gamma",
    //        "Richard Helm",
    //        "Ralph Johnson",
    //        "John Vlissides"
    //    ],
    std::vector<Parser> tmp;
    // 跳过左括号
    EXPECT(v_ptr, '[');
    // 左括号后是一个空白
    parse_whitespace();
    // 遇到数组的右括号，然后将当前字符位置右移一位，并将 Value 设置为数组 tmp
    if (*v_ptr == ']') {
        v_ptr++;
        type_ = L_ARRAY;
        arr_ = tmp;
        return L_PARSE_OK;
    }
    while (1) {
        // 开始parse value
        return_type res = parse_value();
        if (res != L_PARSE_OK) {
            type_ = L_None;
            return res;
        }
        // 将当前对象加入tmp
        tmp.push_back(*this);
        // 有时候逗号前面也会有空格
        parse_whitespace();
        if (*v_ptr == ',') {
            v_ptr++;
            // 逗号后面的空格
            parse_whitespace();
        } else if (*v_ptr == ']') {
            // [ 1, 2, 3,] 不合法，所以要么判断下一个为,要么判断下一个为]，不能分开写两个if
            v_ptr++;
            type_ = L_ARRAY;
            arr_ = tmp;
            return L_PARSE_OK;
        } else {
            type_ = L_None;
            return L_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
        }
    }
}

return_type Parser::parse_object() {
    // { "n" : null , "f" : false }
    std::unordered_map<std::string, Parser> tmp_dict;
    std::string key;
    EXPECT(v_ptr, '{');
    parse_whitespace();
    if (*v_ptr == '}') {
        v_ptr++;
        type_ = L_OBJECT;
        dict = tmp_dict;
        return L_PARSE_OK;
    }
    while (1) {
        if (*v_ptr != '\"') {
            return L_PARSE_MISS_KEY;
        }
        parse_string_raw(key);
        parse_whitespace(); // 冒号前的空白
        if (*v_ptr++ != ':') {
            return L_PARSE_MISS_COLON;
        }
        parse_whitespace();
        return_type res = parse_value();
        if (res != L_PARSE_OK) {
            type_ = L_None;
            return res;
        }
        // 将当前的key和对象加入tmp_dict
        tmp_dict.insert(std::pair<std::string, Parser>(key, *this));
        // 把之前的key给clear了
        key.clear();

        // 逗号前面的
        parse_whitespace();
        if (*v_ptr == ',') {
            v_ptr++;
            parse_whitespace();
        } else if (*v_ptr == '}') {
            v_ptr++;
            type_ = L_OBJECT;
            dict = tmp_dict;
            return L_PARSE_OK;
        } else {
            type_ = L_None;
            return L_PARSE_MISS_COMMA_OR_CURLY_BRACKET;
        }
    }
}

