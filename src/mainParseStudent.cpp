//
// Created by 白天宇 on 2024/4/22.
//
#include "../include/parser.h"
#include "../include/student_map.h"

using namespace json;

int main() {
    std::string json_str = "{\n"
                           "    \"age\": 12,\n"
                           "    \"number\": 1234,\n"
                           "    \"name\": \"CrisHarries\",\n"
                           "    \"phone\": \"c15831345556\",\n"
                           "    \"address\": \"Xi'an\",\n"
                           "    \"school\": \"xidianU\",\n"
                           "    \"major\": \"computerScience\",\n"
                           "    \"grade\": \"forth\"\n"
                           "}";
    Parser parser;
    parser.parse(json_str);
    Student student;
    for (auto &m: student.get_members<Student>()) {
        Parser tmp_parse = parser.get_dict_().at(m.first);
        lept_type val_type = tmp_parse.get_type_();
        if (val_type == L_STRING) {
            student.set_member_by_key(student, m.first, tmp_parse.get_string_());
        } else if (val_type == L_NUMBER) {
            student.set_member_by_key(student, m.first, int(tmp_parse.get_num_()));
        }
    }
    std::cout << student.getAge() << std::endl;
    std::cout << student.getNumber() << std::endl;
    std::cout << student.getName() << std::endl;
    std::cout << student.getPhone() << std::endl;
    std::cout << student.getAddress() << std::endl;
    std::cout << student.getSchool() << std::endl;
    std::cout << student.getMajor() << std::endl;
    std::cout << student.getGrade() << std::endl;
}