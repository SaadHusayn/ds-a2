#ifndef RECORD_H
#define RECORD_H

#include<iostream>

class Record {
    public:
    int id;
    std::string name;
    int age;


    Record(int _id=0, std::string _name="", int _age=0) : id(_id), name(_name), age(_age) {}
};


#endif