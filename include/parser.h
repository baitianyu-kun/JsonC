//
// Created by 白天宇 on 2024/4/3.
//

#ifndef MYJSON5_PARSER_H
#define MYJSON5_PARSER_H

#include "string"
#include "status.h"

using namespace json;

class Parser {

private:
    const char *v_ptr;

    lept_type type_;
    double num_;
    std::string str_;

    void parse_whitespace();

    return_type parse_value();

    // 数字中带非数字的其他字符串怎么办
    return_type parse_number();

    return_type parse_string();

    return_type parse_hex4(const char *&p, unsigned &u);

    void parse_encode_utf8(std::string &tmp, unsigned &u);


    // 将null true false进行合并
    return_type parse_liter(const char *lit, lept_type cur_type);

public:

    return_type parse(const std::string &str);

    lept_type get_type_();

    double get_num_();

    const std::string &get_string_();

    void seeALLStr();

};

#endif //MYJSON5_PARSER_H
