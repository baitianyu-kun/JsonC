//
// Created by 白天宇 on 2024/4/22.
//

#ifndef JSONC_STUDENT_H
#define JSONC_STUDENT_H

//
// Created by 白天宇 on 2024/4/22.
//
#include "../include/parser.h"
#include "iostream"
#include "vector"
#include "map"
#include <cstddef>
#include <typeinfo>

using namespace json;

#define varName(x) #x
#define MEMBER(T, M) { #M, typeid(member_type(&T::M)), offsetof(T, M) }

struct Member {
    const char *name;
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
    std::vector<Member> members;
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
        members.push_back(MEMBER(Student, age));
        members.push_back(MEMBER(Student, number));
        members.push_back(MEMBER(Student, name));
        members.push_back(MEMBER(Student, phone));
        members.push_back(MEMBER(Student, address));
        members.push_back(MEMBER(Student, school));
        members.push_back(MEMBER(Student, major));
        members.push_back(MEMBER(Student, grade));
    };

    template<class T>
    std::vector<Member> &get_members();

    template<>
    std::vector<Member> &get_members<Student>() {
        return members;
    };

    template<class M, class T>
    M &get_member_by_index(T &a, size_t index) {
        return *reinterpret_cast<M *>(reinterpret_cast<char *>(&a) + get_members<T>()[index].offset);
    };

    template<class M, class T>
    void set_member_by_index(T &a, size_t index, M value) {
        *reinterpret_cast<M *>(reinterpret_cast<char *>(&a) + get_members<T>()[index].offset) = value;
    };


    int getAge() const {
        return age;
    }

    void setAge(int age) {
        Student::age = age;
    }

    int getNumber() const {
        return number;
    }

    void setNumber(int number) {
        Student::number = number;
    }

    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
        Student::name = name;
    }

    const std::string &getPhone() const {
        return phone;
    }

    void setPhone(const std::string &phone) {
        Student::phone = phone;
    }

    const std::string &getAddress() const {
        return address;
    }

    void setAddress(const std::string &address) {
        Student::address = address;
    }

    const std::string &getSchool() const {
        return school;
    }

    void setSchool(const std::string &school) {
        Student::school = school;
    }

    const std::string &getMajor() const {
        return major;
    }

    void setMajor(const std::string &major) {
        Student::major = major;
    }

    const std::string &getGrade() const {
        return grade;
    }

    void setGrade(const std::string &grade) {
        Student::grade = grade;
    }
};


#endif //JSONC_STUDENT_H
