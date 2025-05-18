#include "muduo/base/Timestamp.h"
#include "muduo/net/tcp/Callback.h"
#include "muduo/net/tcp/TcpClient.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/tcp/InetAddress.h"
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
        std::cout << "1.所有用户 " << "2.添加用户" << std::endl;
        std::cout << "输入q退出" << std::endl;
        std::cout << "*************************************" << std::endl;
    }

    void inputThread(const TcpConnectionPtr& conn)
    {
        std::string line;
        while (true) {
            order();
            if (getline(std::cin, line)) break;
            std::string msg = line;
            if (msg == "q") break;
            conn->getLoop()->runInLoop([conn, msg]() { conn->send(msg); });
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
