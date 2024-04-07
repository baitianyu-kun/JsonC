//
// Created by 白天宇 on 2024/4/3.
//
#include "iostream"
#include "../include/json_impl.h"

using namespace json;
static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(expect, actual) \
    do {\
        ++test_count;\
        if (expect == actual)\
            ++test_pass;\
        else {\
            std::cerr << __FILE__ << ":" << __LINE__ << ": expect: " << expect << " actual: " << actual << std::endl;\
            main_ret = 1;\
        }\
    } while(0)

#define TEST_LITER(expect_type, expect_status, json_str)\
    do {\
        Json json1;\
        return_type status = json1.parse_json(json_str);\
        lept_type type = json1.get_type();\
        EXPECT_EQ_BASE(expect_type, type);\
        EXPECT_EQ_BASE(expect_status, status);\
    } while (0)

#define TEST_NUM(expect_num, json_str) \
    do{\
        Json json1;\
        return_type status = json1.parse_json(json_str);\
        double num = json1.get_number();\
        EXPECT_EQ_BASE(expect_num, num);\
    }while(0)

#define TEST_NUM_ERROR(expect_status, json_str) \
    do{\
        Json json1;\
        EXPECT_EQ_BASE(expect_status, json1.parse_json(json_str));\
    }while(0)

#define TEST_STR(expect_str, json_str) \
    do{\
        Json json1;\
        return_type status = json1.parse_json(json_str); \
        std::string str = json1.get_string();\
        EXPECT_EQ_BASE(expect_str, str);\
    }while(0)

#define TEST_STR_ERROR(expect_status, json_str) \
    do{\
        Json json1;\
        EXPECT_EQ_BASE(expect_status, json1.parse_json(json_str));\
    }while(0)

static void TEST_STRS() {
    // \x是转义字符，告诉编译器需要用特殊的方式进行处理。 \x表示后面的字符是十六进制数，\0表示后面的字符是八进制数。
    TEST_STR("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"");
    TEST_STR("", "\"\"");
    TEST_STR("Hello", "\"Hello\"");
    TEST_STR("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_STR("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
    TEST_STR("\x24", "\"\\u0024\"");         /* Dollar sign U+0024 */
    TEST_STR("\xC2\xA2", "\"\\u00A2\"");     /* Cents sign U+00A2 */
    TEST_STR("\xE2\x82\xAC", "\"\\u20AC\""); /* Euro sign U+20AC */
    TEST_STR("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"");  /* G clef sign U+1D11E */
    TEST_STR("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"");  /* G clef sign U+1D11E */
}

static void TEST_NUMS() {
    TEST_NUM(0.0, "0");
    TEST_NUM(0.0, "-0");
    TEST_NUM(0.0, "-0.0");
    TEST_NUM(1.0, "1");
    TEST_NUM(-1.0, "-1");
    TEST_NUM(1.5, "1.5");
    TEST_NUM(-1.5, "-1.5");
    TEST_NUM(3.1416, "3.1416");
    TEST_NUM(1E10, "1E10");
    TEST_NUM(1e10, "1e10");
    TEST_NUM(1E+10, "1E+10");
    TEST_NUM(1E-10, "1E-10");
    TEST_NUM(-1E10, "-1E10");
    TEST_NUM(-1e10, "-1e10");
    TEST_NUM(-1E+10, "-1E+10");
    TEST_NUM(-1E-10, "-1E-10");
    TEST_NUM(1.234E+10, "1.234E+10");
    TEST_NUM(1.234E-10, "1.234E-10");
    TEST_NUM(0.0, "1e-10000");
    TEST_NUM_ERROR(L_PARSE_INVALID_VALUE, "+0");
    TEST_NUM_ERROR(L_PARSE_INVALID_VALUE, "+1");
    TEST_NUM_ERROR(L_PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
    TEST_NUM_ERROR(L_PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
    TEST_NUM_ERROR(L_PARSE_INVALID_VALUE, "INF");
    TEST_NUM_ERROR(L_PARSE_INVALID_VALUE, "inf");
    TEST_NUM_ERROR(L_PARSE_INVALID_VALUE, "NAN");
    TEST_NUM_ERROR(L_PARSE_INVALID_VALUE, "nan");
}

static void TEST_NULL_TRUE_FALSE() {
    TEST_LITER(L_TRUE, L_PARSE_OK, "true");
    TEST_LITER(L_FALSE, L_PARSE_OK, "false");
    TEST_LITER(L_NULL, L_PARSE_OK, "null");
    TEST_LITER(L_NULL, L_PARSE_OK, "null ");
    TEST_LITER(L_None, L_PARSE_ROOT_NOT_SINGULAR, "null 1");
    TEST_LITER(L_None, L_PARSE_ROOT_NOT_SINGULAR, "null1");
    TEST_LITER(L_None, L_PARSE_EXPECT_VALUE, "");
    TEST_LITER(L_None, L_PARSE_EXPECT_VALUE, " ");
    TEST_LITER(L_None, L_PARSE_INVALID_VALUE, "nall");
    TEST_LITER(L_None, L_PARSE_INVALID_VALUE, "felse");
    TEST_LITER(L_None, L_PARSE_INVALID_VALUE, "tree");
}

int main() {
//    TEST_NULL_TRUE_FALSE();
//    TEST_NUMS();
    TEST_STRS();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}