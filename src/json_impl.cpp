//
// Created by 白天宇 on 2024/4/3.
//

#include "iostream"
#include "string"
#include "../include/json_impl.h"

using namespace json;

return_type Json::parse_json(const std::string &json_str) {
    return_type res = parser.parse(json_str);
    type = parser.get_type_();
    return res;
}

lept_type Json::get_type() {
    return type;
}

double Json::get_number() {
    num = parser.get_num_();
    return num;
}

const std::string &Json::get_string() {
    str = parser.get_string_();
    return str;
}
