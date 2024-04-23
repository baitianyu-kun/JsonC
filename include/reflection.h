//
// Created by 白天宇 on 2024/4/22.
//

#ifndef JSONC_REFLECTION_H
#define JSONC_REFLECTION_H

#include "iostream"

#define varName(x) #x
#define MEMBER(T, M) { typeid(member_type(&T::M)), offsetof(T, M) }

template<class T, class M>
M member_type(M T::*) {};

struct Member {
    const std::type_info &type;
    std::size_t offset;
};

class Reflection {
protected:
    std::unordered_map<std::string, Member> members;
public:
    virtual void set_members() {};

    template<class T>
    std::unordered_map<std::string, Member> &get_members() {
        return members;
    };

    template<class M, class T>
    M &get_member_by_key(T &a, std::string key) {
        // reinterpret_cast<char *>(&a)目的是为了以字节为单位访问对象的内存表示。将对象 a 的地址视为指向字符类型的指针，
        // 这样可以绕过类型系统的限制，将对象的内存视为字节序列。这是因为字符类型（char）的大小为一个字节，而其他类型（如整数、浮点数等）的大小可能不止一个字节。
        // 通过将地址视为字符类型的指针，我们可以使用字节级别的操作来访问对象的内存。这在某些情况下是有用的，例如在进行底层操作、内存布局分析或者进行类型转换时。
        // 所以可以适应各种类型比如int啥的
        // 经用户名为“hall919”的朋友提醒，
        // 他在ubuntu 18.04，64位 环境下测试，long占据8个字节。网上搜索发现，long占据的字节数还和编译器的数据模型相关
        return *reinterpret_cast<M *>(reinterpret_cast<char *>(&a) + get_members<T>().at(key).offset);
    };

    template<class M, class T>
    void set_member_by_key(T &a, std::string key, M value) {
        *reinterpret_cast<M *>(reinterpret_cast<char *>(&a) + get_members<T>().at(key).offset) = value;
    };
};


#endif //JSONC_REFLECTION_H
