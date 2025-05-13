#include "main.h"
#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/mysql_driver.h>
#include <memory>
#include <mysql/mysql.h>
#include <mysql_driver.h>

StudentManage::StudentManage()
{
    sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();

    std::unique_ptr<sql::Connection> conn(
        driver->connect("tcp://127.0.0.1:3306", "root", "123456")
    );

}

void StudentManage::insert_student(Student& stu)
{
    try {
        conn->setSchema("test_database");

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("insert into student(id, name) values (?, ?)")
        );
    
        pstmt->setInt(1,stu.getid());
        pstmt->setString(2, stu.getname());
    
        pstmt->execute();
    
        std::cout<< "intsert_student successful. " << std::endl;
    } catch (sql::SQLException& e) {
        std::cout<< "SQL error: " << e.what() << std::endl;
    }
}

void StudentManage::update_student(Student& stu)
{
    try {
        conn->setSchema("test_datebase");

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("update student set name = (?) where id = (?)")
        );

        pstmt->setString(1, stu.getname());
        pstmt->setInt(2, stu.getid());

        pstmt->execute();

        std::cout<< "update_student successful. " << std::endl;
    } catch (sql::SQLException& e) {
        std::cout<< "SQL error: " << e.what() << std::endl;
    }
}

void StudentManage::delete_student(Student& stu)
{
    try {
        conn->setSchema("test_datebase");

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("delete from student where id = (?)")
        );

        pstmt->setInt(1, stu.getid());

        pstmt->execute();

        std::cout<< "delete_student successful. " << std::endl;
    } catch (sql::SQLException& e) {
        std::cout<< "SQL error: " << e.what() << std::endl;
    }
}