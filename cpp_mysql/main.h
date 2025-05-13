#ifndef MAIN_H
#define MAIN_H

#include <jdbc/cppconn/connection.h>
#include <memory>
#include <mysql/jdbc.h>
#include <mysql/mysql.h>
#include <string>
#include <vector>

class Student 
{
public:
    Student(int id, std::string name) : id_(id), name_(name)
    {}
    ~Student();

    int getid() { return id_; }
    std::string getname() { return name_; }

private:
    int id_;
    std::string name_;
};

class StudentManage
{
public:
    StudentManage();
    ~StudentManage();

    void insert_student(Student& t);
    void update_student(Student& t);
    void delete_student(Student& t);
    std::vector<Student> get_student(std::string condition = "");

private:
    std::unique_ptr<sql::Connection> conn;
    const char* host = "127.0.0.1";
    const char* user = "root";
    const char* pw = "123456";
    const char* datebase_name = "datebase_test";
    const int port = 3306;
};



#endif