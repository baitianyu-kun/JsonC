//
// Created by 白天宇 on 2024/4/22.
//
#include "student_map.h"

using namespace json;

int main() {
//    Student student;
//    for (auto &m: student.get_members<Student>()) {
//        std::cout << m.name << " " << m.type.name() << " " << m.offset << std::endl;
//    }
//    std::cout << student.get_members<Student>()[0].offset << std::endl;
//    std::cout << student.get_member_by_index<int>(student, 0)<<std::endl;
//    student.set_member_by_index(student, 0, 1999);
//    std::cout << student.get_member_by_index<int>(student, 0);

    Student student;
//    for (auto &m: student.get_members<Student>()) {
//        std::cout << m.first << " " << m.second.type.name() << " " << m.second.offset << std::endl;
//    }
//    std::cout << student.getAge() << std::endl;
    student.set_member_by_key(student, "age", 1999);
    student.set_member_by_key(student, "number", 19991111);
//    std::cout << student.getAge() << std::endl;
//    std::cout << student.getNumber() << std::endl;
}