#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "defaults.hpp"
#include <cstring>
#include <functional>
#include <mutex>
#include <netinet/in.h>
#include <print>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>

namespace HTTP {

class Server {
public:
  static auto getInstance(int port = 0) -> Server * {
    std::lock_guard<std::mutex> lock(mtx);

    if (instance == nullptr) {
      instance = new Server(port);
    }

    return instance;
  }

  auto addRoute(const std::string &path, std::function<void(int fd)> handler)
      -> void;
  auto start() -> void;
  auto getHeaders() const -> HTTP::Headers { return headers; }

  Server(const Server &) = delete;
  Server &operator=(const Server &) = delete;

private:
  static Server *instance;
  static std::mutex mtx;

  Server(int port);
  ~Server();

  auto handleRequest(int fd) -> void;
  std::unordered_map<std::string, std::function<void(int fd)>> routes;
  int server_fd;
  std::string ip;
  HTTP::Headers headers = HTTP::Headers{};
  auto loadHeaders(int fd) -> void;
};

} // namespace HTTP
#endif // HTTP_SERVER_H
