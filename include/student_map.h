//
// Created by 白天宇 on 2024/4/22.
//

#ifndef JSONC_STUDENT_MAP_H
#define JSONC_STUDENT_MAP_H

#include "../include/parser.h"
#include "iostream"
#include "vector"
#include "map"
#include <cstddef>
#include <typeinfo>

using namespace json;

#define varName(x) #x

#define MEMBER(T, M) { typeid(member_type(&T::M)), offsetof(T, M) }

struct Member {
    const std::type_info &type;
    std::size_t offset;
};

class Student {
private:
    int age = 12;
    int number = 1234;
    std::string name = "baitainyu";
    std::string phone;
    std::string address;
    std::string school;
    std::string major;
    std::string grade;
    std::unordered_map<std::string, Member> members;
public:

    Student() {
        set_members<Student>();
    }

    template<class T, class M>
    M member_type(M T::*) {};

    template<class T>
    void set_members();

    template<>
    void set_members<Student>() {
        members.insert(std::pair<std::string, Member>(varName(age), MEMBER(Student, age)));
        members.insert(std::pair<std::string, Member>(varName(number), MEMBER(Student, number)));
        members.insert(std::pair<std::string, Member>(varName(name), MEMBER(Student, name)));
        members.insert(std::pair<std::string, Member>(varName(phone), MEMBER(Student, phone)));
        members.insert(std::pair<std::string, Member>(varName(address), MEMBER(Student, address)));
        members.insert(std::pair<std::string, Member>(varName(school), MEMBER(Student, school)));
        members.insert(std::pair<std::string, Member>(varName(major), MEMBER(Student, major)));
        members.insert(std::pair<std::string, Member>(varName(grade), MEMBER(Student, grade)));
    };

    template<class T>
    std::unordered_map<std::string, Member> &get_members();

    template<>
    std::unordered_map<std::string, Member> &get_members<Student>() {
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

    int getAge() const {
        return age;
    }

    int getNumber() const {
        return number;
    }

    const std::string &getName() const {
        return name;
    }

    const std::string &getPhone() const {
        return phone;
    }

    const std::string &getAddress() const {
        return address;
    }

    const std::string &getSchool() const {
        return school;
    }

    const std::string &getMajor() const {
        return major;
    }

    const std::string &getGrade() const {
        return grade;
    }
};

#endif //JSONC_STUDENT_MAP_H
