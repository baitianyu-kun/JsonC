//
// Created by 白天宇 on 2024/4/3.
//

#include "iostream"
#include "string"
#include "../include/json_impl.h"

using namespace json;

return_type Json::parse_json(const std::string &json_str) {
    return_type res = parser.parse(json_str);
    return res;
}

const lept_type &Json::get_type() {
    return parser.get_type_();
}

const double &Json::get_number() {
    return parser.get_num_();
}

const std::string &Json::get_string() {
    return parser.get_string_();
}

const std::vector<Parser> &Json::get_arr() {
    return parser.get_arr_();
}