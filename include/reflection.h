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
        return *reinterpret_cast<M *>(reinterpret_cast<char *>(&a) + get_members<T>().at(key).offset);
    };

    template<class M, class T>
    void set_member_by_key(T &a, std::string key, M value) {
        *reinterpret_cast<M *>(reinterpret_cast<char *>(&a) + get_members<T>().at(key).offset) = value;
    };
};


#endif //JSONC_REFLECTION_H
