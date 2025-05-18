#include "muduo/base/Timestamp.h"
#include "muduo/net/Buffer.h"
#include "muduo/net/tcp/Callback.h"
#include "muduo/net/tcp/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/tcp/InetAddress.h"

#include <cstdio>
#include <functional>
#include <nlohmann/json.hpp>
#include <mysql/mysql.h>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <sstream>
#include <string>

using json = nlohmann::json;

class Usrservice 
{
public:
    Usrservice(EventLoop* loop, InetAddress& addr, std::string name) 
        : loop_(loop),
          server_(loop, addr, name)
    {
        mysql = mysql_init(nullptr);
        if (!mysql) {
            std::cerr << "MYSQL init failed\n" ;
            return ;
        }
        if (!mysql_real_connect(mysql, "localhost", "hahaha", "123456", "uer", 3306, nullptr, 0))
        {
            std::cerr << "Connection failed: " << mysql_error(mysql) << std::endl;
        }
        server_.setConnectionCallback(std::bind(&Usrservice::Connectioncallback, this, std::placeholders::_1));
        server_.setMessageCallback(std::bind(&Usrservice::Messagecallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        server_.setWriteCompleteCallback(std::bind(&Usrservice::Writecompletecallback, this, std::placeholders::_1));
    }

    ~Usrservice(){
        mysql_close(mysql);
    }

    void setThreadnum(int num)
    {
        server_.setThreadNum(num);
    }

    void start()
    {
        server_.start();
    }

private:

    void Connectioncallback(const TcpConnectionPtr& conn)
    {
        if (conn->connected())
        {
            std::cout << "New Connection:" << conn->peerAddress().toIpPort() << std::endl;
        }
        else 
        {
            std::cout << "Connection closed:" << conn->peerAddress().toIpPort() << std::endl;
        }
    }

    void getallusr(const TcpConnectionPtr& conn)
    {
        std::string sendmsg;
        const char* sql = "select json_data from usr_name";
        mysql_query(mysql, sql);
        MYSQL_RES* result = mysql_store_result(mysql);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(result)))
        {
            if (row[0])
            {
                std::string json_str = row[0];

                try {
                    json parsed = json::parse(json_str);
                    sendmsg += std::to_string(parsed["id"].get<int>());
                    sendmsg += "  ";
                    sendmsg += parsed["name"];
                    sendmsg += "\n";
                } catch (const json::exception &e) {
                    std::cerr << "JSON parsing error : "  << e.what() << '\n';
                }
            }
        }
        conn->getLoop()->runInLoop([conn, sendmsg]() { conn->send(sendmsg); });
        mysql_free_result(result);
    }

    void insertusr(std::string& str)
    {
        json parsed = json::parse(str);
        int id = parsed["id"];
        std::string name = parsed["name"];
        
        json j;
        j["id"] = id;
        j["name"] =name;
        std::string json_str = j.dump();

        MYSQL_STMT* stmt = mysql_stmt_init(mysql);
        const char* sql = "insert into usr_name (json_data) values (?)";
        mysql_stmt_prepare(stmt, sql, strlen(sql));

        MYSQL_BIND bind[1];
        memset(bind, 0, sizeof(bind));

        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = (void*)json_str.data();
        bind[0].buffer_length = json_str.size();

        mysql_stmt_bind_param(stmt, bind);
        if (mysql_stmt_execute(stmt))
        {
            std::cerr << "Insert failed: " << mysql_stmt_error(stmt) << "\n";
        }

        mysql_stmt_close(stmt);
    }

    void Messagecallback(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
    {
        std::string str = buf->retrieveAllAsString();
        try {
            json j = json::parse(str);
            int op = j["op"];
            if (op == 1)
            {
                printf("-------------------\n");
                getallusr(conn);
            }
            else if (op == 2)
            {
                insertusr(str);
            }
            else 
            {
                std::string errormgs("Wrong message format\n");
                conn->getLoop()->runInLoop([conn, errormgs]() { conn->send(errormgs); });
            }
        } catch (const std::exception& e) {
            std::string errormsg = std::string("Parse error: ") + e.what() + "\n";
            conn->getLoop()->runInLoop([conn, errormsg]() { conn->send(errormsg); });
        }
    }
    void Writecompletecallback(const TcpConnectionPtr& conn)
    {
    }

    MYSQL *mysql;
    EventLoop* loop_;
    TcpServer server_;
};

int main() {
    EventLoop loop;
    InetAddress inet(8080);
    Usrservice ser(&loop, inet, "server");
    ser.setThreadnum(5);

    ser.start();
    loop.loop();
    return 0;
}