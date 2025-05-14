#include <sw/redis++/redis++.h>
#include <iostream>
#include <string>  // 显式包含 string（对老系统更稳妥）

int main() {
    try {
        // 创建 Redis 客户端连接（默认端口 6379）
        sw::redis::Redis redis("tcp://127.0.0.1:6379");

        // 使用 std::string_view（C++17 起支持）
        const std::string_view key = "name";
        const std::string_view value = "ChatGPT";

        // 设置键值对
        redis.set(key, value);

        // 获取键值对
        auto val = redis.get(key);

        // 判断是否成功获取
        if (val) {
            std::cout << "Value for key '" << key << "': " << *val << std::endl;
        } else {
            std::cout << "Key not found." << std::endl;
        }

    } catch (const sw::redis::Error &e) {
        std::cerr << "Redis error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
