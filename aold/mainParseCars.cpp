//
// Created by 白天宇 on 2024/4/22.
//
#include "../include/cars.h"
#include "../include/parser.h"
#include "iostream"
using namespace json;


int main(){
    std::string json_str = "{\n"
                           "    \"age\": 12,\n"
                           "    \"number\": 12334,\n"
                           "    \"models\": \"tesla\",\n"
                           "    \"address\": \"Xi'anYT\",\n"
                           "    \"price\": \"12999$\"\n"
                           "}";
    Parser parser;
    parser.parse(json_str);
    Cars cars;
    parser.setResult(cars);
    std::cout<<cars.getAge()<<std::endl;
    std::cout<<cars.getModels()<<std::endl;
    std::cout<<cars.getAddress()<<std::endl;
    std::cout<<cars.getNumber()<<std::endl;
    std::cout<<cars.getPrice()<<std::endl;
}