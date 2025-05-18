#include "muduo/base/Timestamp.h"
#include "muduo/net/tcp/Callback.h"
#include "muduo/net/tcp/TcpClient.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/tcp/InetAddress.h"

#include <nlohmann/json.hpp>
#include <cstddef>
#include <functional>
#include <thread>
#include <ctime>
#include <iostream>


class UsrSystem 
{
public:
    UsrSystem(EventLoop* loop, const InetAddress& addr, const std::string& name)
        : loop_(loop),
          client_(loop,addr,name)
    {
        client_.setConnectionCallback(std::bind(&UsrSystem::Connectioncallback, this, std::placeholders::_1));
        client_.setMessageCallback(std::bind(&UsrSystem::Messagecallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        client_.setWriteCompleteCallback(std::bind(&UsrSystem::Writecompletecallback, this, std::placeholders::_1));
    }
    ~UsrSystem(){}

    void start() {
        client_.connect();
    }

private:
    void order()
    {
        std::cout << "*************************************" << std::endl;
        std::cout << "1.输出所有用户 " << "2.添加用户" << std::endl;
        std::cout << "输入q退出" << std::endl;
        std::cout << "*************************************" << std::endl;
    }

    void inputThread(const TcpConnectionPtr& conn)
    {
        std::string line;
        while (true) {
            order();
            if (!std::getline(std::cin, line)) break;

            if (line == "q") break;

            nlohmann::json msg;

            if (line == "1") {
                msg["op"] = 1;
            } else if (line == "2") {
                msg["op"] = 2;
                std::cout << "Enter id: ";
                int id;
                std::cin >> id;
                std::cin.ignore(); // 丢掉换行符

                std::cout << "Enter name: ";
                std::string name;
                std::getline(std::cin, name);

                msg["id"] = id;
                msg["name"] = name;
            } else {
                std::cout << "Unknown command\n";
                continue;
            }

            std::string sendmsg = msg.dump();
            conn->getLoop()->runInLoop([conn, sendmsg]() { conn->send(sendmsg); });
        }
    }

    void Connectioncallback(const TcpConnectionPtr& conn)
    {
        if (conn->connected())
        {
            std::cout << "Connection successful" << std::endl;
            std::thread t([this, conn]() { this->inputThread(conn); });
            t.detach();
        }
        else  
        {
            std::cout << "Connection closed" << std::endl;
        }
    }

    void Messagecallback(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
    {
        std::string msg(buf->retrieveAllAsString());
        std::cout << "From server : " << std::endl << msg << std::endl;
    }

    void Writecompletecallback(const TcpConnectionPtr& conn) 
    {

    }

    EventLoop* loop_;
    TcpClient client_;
};


int main() {
    EventLoop loop;
    InetAddress inet(8080, "127.0.0.1");

    UsrSystem client(&loop, inet, "client");

    client.start();
    loop.loop();
    return 0;
}
