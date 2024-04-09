//
// Created by 白天宇 on 2024/4/3.
//

#ifndef MYJSON5_PARSER_H
#define MYJSON5_PARSER_H

#include "string"
#include "status.h"
#include "vector"
#include "unordered_map"

using namespace json;

class Parser {

private:
    const char *v_ptr;

    lept_type type_;
    double num_;
    std::string str_;
    // vector里既有可能是num，也有可能是str
    std::vector<Parser> arr_;
    std::unordered_map<std::string, Parser> dict_;

    void parse_whitespace();

    return_type parse_value();

    // 数字中带非数字的其他字符串怎么办
    return_type parse_number();

    return_type parse_string();

    return_type parse_string_raw(std::string &tmp);

    return_type parse_hex4(const char *&p, unsigned &u);

    void parse_encode_utf8(std::string &tmp, unsigned &u);

    // 将null true false进行合并
    return_type parse_liter(const char *lit, lept_type cur_type);

    return_type parse_array();

    return_type parse_object();

    void stringify_string(std::string &json_str_save, const std::string &str_need_stringify);


public:

    return_type parse(const std::string &str);

    void stringify(std::string &str);

    const lept_type &get_type_() const;

    const double &get_num_() const;

    const std::string &get_string_() const;

    // 这个函数如果返回const std::vector<Parser> &get_arr_的话，则所有的get_type_(),get_num_()等需要在后面加const
    // 比如get_arr_[0]，其返回的是一个Parser const的this指针，即返回一个常量对象const Parser，所以调用的函数也必须是
    // 常量函数，即在函数后加const。
    // 然而，'get_type_' 函数在声明中没有被标记为常量成员函数，这导致了报错
    // 因为常量对象上的成员函数默认情况下是常量成员函数，只能访问对象的成员，不能修改对象的状态。
    // 为了解决这个问题，你可以将 'get_type_' 函数声明为常量成员函数，通过在函数声明和定义的末尾加上 'const' 关键字，
    const std::vector<Parser> &get_arr_() const;

    const size_t &get_arr_len() const;

    const Parser &get_arr_element(size_t index) const;

    void set_arr_(std::vector<Parser> &arr);

    void push_arr_(Parser &arr);

    void pop_arr_();

    void insert_arr_(Parser &arr, size_t index);

    // 根据索引删除数组中的某段区间内的元素
    void erase_arr_(size_t index, size_t count);

    void clear_arr_();

    const std::unordered_map<std::string, Parser> &get_dict_() const;

    const size_t &get_dict_len() const;

    const Parser &get_dict_element(std::string &key) const;

    Parser &get_dict_element2(std::string &key);

    void insert_dict_(std::string &key, Parser &dict);

    void erase_dict_(std::string &key);

    void swap_dict_(std::unordered_map<std::string, Parser> &dict);

    void clear_dict_();

    void seeALLStr();

    // 运算符重载，判断两个json解析的树是否相同
    bool operator==(const Parser &other) const;

    bool operator!=(const Parser &other) const;

};

#endif //MYJSON5_PARSER_H
