//
// Created by 白天宇 on 2024/4/3.
//

#ifndef MYJSON5_STATUS_H
#define MYJSON5_STATUS_H
namespace json{
    // L_None: 非法字符
    enum lept_type {
        L_NULL, L_FALSE, L_TRUE, L_NUMBER, L_STRING, L_ARRAY, L_OBJECT, L_None
    };
    enum return_type : int {
        L_PARSE_OK = 0,
        L_PARSE_EXPECT_VALUE,
        L_PARSE_INVALID_VALUE,
        L_PARSE_ROOT_NOT_SINGULAR,
        L_PARSE_NUMBER_TOO_BIG,
        L_PARSE_MISS_QUOTATION_MARK,
        L_PARSE_INVALID_UNICODE_HEX,
        L_PARSE_INVALID_UNICODE_SURROGATE,
        L_PARSE_INVALID_STRING_ESCAPE,
        L_PARSE_INVALID_STRING_CHAR
    };
}
#endif //MYJSON5_STATUS_H
