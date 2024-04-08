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

#define TEST_ROUNDTRIP(json_str) \
    do{\
        Parser parser;\
        parser.parse(json_str);\
        std::string json_str_stringify = "";\
        parser.stringify(json_str_stringify);\
        EXPECT_EQ_BASE(json_str, json_str_stringify);\
    }while(0)

static void TEST_ARR() {
    std::string vec_str1 = "[ null , false , true , 0 , 1 , \"abc\" ]";
    std::string vec_str2 = "[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] , [ \"abc\" , 12 ] ]";
    std::string vec_str3 = "[ ]";
    Json json1;
    json1.parse_json(vec_str1);
    EXPECT_EQ_BASE(L_NULL, json1.get_arr()[0].get_type_());
    EXPECT_EQ_BASE(L_FALSE, json1.get_arr()[1].get_type_());
    EXPECT_EQ_BASE(L_TRUE, json1.get_arr()[2].get_type_());
    EXPECT_EQ_BASE(0, json1.get_arr()[3].get_num_());
    EXPECT_EQ_BASE(1, json1.get_arr()[4].get_num_());
    EXPECT_EQ_BASE("abc", json1.get_arr()[5].get_string_());

    json1.parse_json(vec_str2);
    EXPECT_EQ_BASE(L_ARRAY, json1.get_arr()[0].get_type_());
    EXPECT_EQ_BASE(L_ARRAY, json1.get_arr()[1].get_type_());
    EXPECT_EQ_BASE(L_ARRAY, json1.get_arr()[2].get_type_());
    EXPECT_EQ_BASE(L_ARRAY, json1.get_arr()[3].get_type_());
    EXPECT_EQ_BASE(0, json1.get_arr()[3].get_arr_()[0].get_num_());
    EXPECT_EQ_BASE(1, json1.get_arr()[3].get_arr_()[1].get_num_());
    EXPECT_EQ_BASE(2, json1.get_arr()[3].get_arr_()[2].get_num_());
    EXPECT_EQ_BASE("abc", json1.get_arr()[4].get_arr_()[0].get_string_());
    EXPECT_EQ_BASE(12, json1.get_arr()[4].get_arr_()[1].get_num_());

    json1.parse_json(vec_str3);
    EXPECT_EQ_BASE(0, json1.get_arr().size());

//    std::string vec_str1 = "[ \"abc\" ]";
//    Json json1;
//    json1.parse_json(vec_str1);
//    EXPECT_EQ_BASE("abc",json1.get_arr()[0].get_string_());

}

static void TEST_OBJ() {
    Parser parser;
    std::string objs = " { "
                       "\"n\" : null , "
                       "\"f\" : false , "
                       "\"t\" : true , "
                       "\"i\" : 123 , "
                       "\"s\" : \"abc\", "
                       "\"a\" : [ 1, 2, 3 ],"
                       "\"o\" : { \"1\" : 1, \"2\" : 2, \"3\" : 3 }"
                       " } ";
    // { "n" : null , "f" : false , "t" : true , "i" : 123 , "s" : "abc", "a" : [ 1, 2, 3 ],"o" : { "1" : 1, "2" : 2, "3" : 3 } }
    parser.parse(objs);
    std::unordered_map<std::string, Parser> dict = parser.get_dict_();
    EXPECT_EQ_BASE(L_NULL, dict["n"].get_type_());
    EXPECT_EQ_BASE(L_FALSE, dict["f"].get_type_());
    EXPECT_EQ_BASE(L_TRUE, dict["t"].get_type_());
    EXPECT_EQ_BASE(123, dict["i"].get_num_());
    EXPECT_EQ_BASE("abc", dict["s"].get_string_());
    EXPECT_EQ_BASE(3, dict["a"].get_arr_().size());
    std::unordered_map<std::string, Parser> dict2 = dict["o"].get_dict_();
    EXPECT_EQ_BASE(1, dict2["1"].get_num_());
}

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


static void TEST_STRINGIFY(){
//    TEST_ROUNDTRIP("0");
//    TEST_ROUNDTRIP("-0");
//    TEST_ROUNDTRIP("1");
//    TEST_ROUNDTRIP("-1");
//    TEST_ROUNDTRIP("1.5");
//    TEST_ROUNDTRIP("-1.5");
//    TEST_ROUNDTRIP("3.25");
//    TEST_ROUNDTRIP("1e+20");
//    TEST_ROUNDTRIP("1.234e+20");
//    TEST_ROUNDTRIP("1.234e-20");
//
//    TEST_ROUNDTRIP("\"\"");
//    TEST_ROUNDTRIP("\"Hello\"");
//    TEST_ROUNDTRIP("\"Hello\\nWorld\"");
//    TEST_ROUNDTRIP("\"\\\" \\\\ / \\b \\f \\n \\r \\t\"");

    std::string json_str = " { "
                           "\"n\" : null"
                           " } ";
    std::string json_str_save = "";
    Parser parser1;
    parser1.parse(json_str);
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
//    TEST_STRS();
//    TEST_ARR();
//    TEST_OBJ();
    TEST_STRINGIFY();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}