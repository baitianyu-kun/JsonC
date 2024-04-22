//
// Created by 白天宇 on 2024/4/22.
//

#ifndef JSONC_CARS_H
#define JSONC_CARS_H

#include "iostream"
#include "reflection.h"

class Cars : public Reflection {
private:
    double age;
    double number;
    std::string models;
    std::string address;
    std::string price;
public:
    Cars() {
        set_members();
    }

    void set_members() override {
        members.insert(std::pair<std::string, Member>(varName(age), MEMBER(Cars, age)));
        members.insert(std::pair<std::string, Member>(varName(number), MEMBER(Cars, number)));
        members.insert(std::pair<std::string, Member>(varName(models), MEMBER(Cars, models)));
        members.insert(std::pair<std::string, Member>(varName(address), MEMBER(Cars, address)));
        members.insert(std::pair<std::string, Member>(varName(price), MEMBER(Cars, price)));
    };

    double getAge() const {
        return age;
    }

    double getNumber() const {
        return number;
    }

    const std::string &getModels() const {
        return models;
    }

    const std::string &getAddress() const {
        return address;
    }

    const std::string &getPrice() const {
        return price;
    }

};

#endif //JSONC_CARS_H
