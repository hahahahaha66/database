#include "muduo/net/tcp/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/tcp/InetAddress.h"



int main() {
    EventLoop loop;
    InetAddress inet(8080);
    TcpServer ser(&loop, inet, "server");

    loop.loop();
    ser.start();
    
    return 0;
}