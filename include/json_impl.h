//
// Created by 白天宇 on 2024/4/3.
//

#ifndef MYJSON5_JSON_IMPL_H
#define MYJSON5_JSON_IMPL_H

#include "iostream"
#include "string"
#include "parser.h"
#include "status.h"

using namespace json;

class Json {
private:
    Parser parser;
public:
    return_type parse_json(const std::string &json_str);

    const double &get_number();

    const lept_type &get_type();

    const std::string &get_string();

    const std::vector<Parser> &get_arr();
};

#endif //MYJSON5_JSON_IMPL_H
