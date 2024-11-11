#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <cstring>
#include <functional>
#include <netinet/in.h>
#include <print>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>

namespace HTTP {

class Server {
public:
  Server(int port);
  ~Server();

  auto addRoute(const std::string &path, std::function<void(int)> handler) -> void;
  auto start() -> void;

private:
  auto handleRequest(int fd) const -> void;
  auto getHeaders(char buffer[1024]) const -> std::unordered_map<std::string, std::string>;
  
  std::unordered_map<std::string, std::function<void(int fd)>> routes;
  int server_fd;
  std::string ip;
};

} // namespace HTTP
#endif // HTTP_SERVER_H
