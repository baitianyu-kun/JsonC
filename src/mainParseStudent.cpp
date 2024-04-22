//
// Created by 白天宇 on 2024/4/22.
//
#include "../include/parser.h"
#include "../include/student.h"

using namespace json;

int main() {
    std::string json_str = "{\n"
                           "    \"age\": 12,\n"
                           "    \"number\": 1234,\n"
                           "    \"name\": \"CrisHarries\",\n"
                           "    \"phone\": \"15831345556\",\n"
                           "    \"address\": \"Xi'an\",\n"
                           "    \"school\": \"xidianU\",\n"
                           "    \"major\": \"computerScience\",\n"
                           "    \"grade\": \"forth\",\n"
                           "}";
    Parser parser;
    parser.parse(json_str);


}